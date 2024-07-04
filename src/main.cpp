#include <Arduboy2.h>
#include "twi.h"

#ifndef __AVR__
#define PSTR(str_) str_
#define PROGMEM
inline uint8_t pgm_read_byte(void const* p) { return *(uint8_t*)p; }
inline uint16_t pgm_read_word(void const* p) { return *(uint16_t*)p; }
inline void const* pgm_read_ptr(void const* p) { return *(void const**)p; }
#endif

#include "utils.h"
#include "maps.h"
#include "images.h"

// high/low byte of uint16_t
#define LO(a) ((uint8_t)(a))
#define HI(a) ((uint8_t)((uint16_t)(a) >> 8))

Arduboy2Base arduboy;

#ifdef FPS_DEBUG
class Arduboy2Ex {
public:
    static bool nextFrameMiniDEV() {
        bool ret = arduboy.nextFrame();
        if (ret) {
            if (arduboy.lastFrameDurationMs > arduboy.eachFrameMillis) {
                arduboy.digitalWriteRGB(RGB_ON, RGB_OFF, RGB_OFF);
            } else {
                arduboy.digitalWriteRGB(RGB_OFF, RGB_ON, RGB_OFF);
            }
        }
        return ret;
    }
};
#endif

// Arduboy framebuffer
uint8_t* buf = arduboy.sBuffer;

// network data
constexpr uint8_t numPlayers = 4;
const uint8_t targetAddresses[numPlayers] PROGMEM = {
    0x00, // dummy data
    0x12,
    0x11,
    0x10
};

uint8_t role;

// private player data
uint16_t orientation = UFIX16(1.5f, 15);    // brad [0,2] in Q15

// (dirX, dirY) is the unit vector derived from orientation
// initialized in setup
int16_t dirX;   // Q15
int16_t dirY;   // Q15

int16_t momX;   // Q16
int16_t momY;   // Q16

constexpr int16_t planeLen = FIX16(0.66f, 15);  // Q15, sets FOV
constexpr int16_t invPlaneLen = FIX16(1/0.66f, 14);  // Q14
constexpr int16_t moveSpeed = FIX16(0.005f, 17); // Q17
constexpr int16_t turnSpeed = FIX16(0.01f, 15); // Q15
constexpr int16_t friction = FIX16(0.9f, 8);    // Q8

uint16_t zbuf[FBW]; // Q8

// public player data / other player data
struct sprite_t {
    uint8_t role;
    uint8_t playerHit;
    int16_t posX;  // Q8
    int16_t posY;  // Q8
};

constexpr uint8_t numSprites = numPlayers;
volatile sprite_t sprites[numSprites];

// line rendering data

constexpr uint8_t SET_MASK[8] = {
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80
};
constexpr uint8_t YMASK0[8] = {
    0xff, 0xfe, 0xfc, 0xf8, 0xf0, 0xe0, 0xc0, 0x80
};
constexpr uint8_t YMASK1[8] = {
    0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff
};

constexpr uint16_t PATTERNS[4] = {
//  0x0000,
    0xaa00,
    0xaa55,
    0xff55,
    0xffff,
};

void draw_vline(uint8_t x, int16_t y0, int16_t y1, uint16_t pat) {
    if (y0 > y1) return;
    if (y1 < 0) return;
    if (y0 >= FBH) return;
    if (x >= FBW) return;

    uint8_t ty0 = (uint8_t)tmax<int16_t>(y0, 0);
    uint8_t ty1 = (uint8_t)tmin<int16_t>(y1, FBH - 1);

    uint8_t t0 = ty0 & 0xf8;
    uint8_t t1 = ty1 & 0xf8;
    uint8_t m0 = YMASK0[ty0 & 0x7];
    uint8_t m1 = YMASK1[ty1 & 0x7];

    uint8_t pattern = (x & 0x1) ? uint8_t(pat) : uint8_t(pat >> 8);

    uint8_t* p = &buf[t0 * (FBW / 8) + x];

    if (t0 == t1) {
        uint8_t m = m0 & m1;
        uint8_t tp = *p;
        tp |= (pattern & m);
        tp &= (pattern | ~m);
        *p = tp;
        return;
    }
    {
        uint8_t m = m0;
        uint8_t tp = *p;
        tp |= (pattern & m);
        tp &= (pattern | ~m);
        *p = tp;
        p += FBW;
    }
    for (int8_t t = t1 - t0 - 8; t > 0; t -= 8) {
        *p = pattern;
        p += FBW;
    }
    {
        uint8_t m = m1;
        uint8_t tp = *p;
        tp |= (pattern & m);
        tp &= (pattern | ~m);
        *p = tp;
    }
}

void rx_event(uint8_t *buffer, int length) {
    if (role != buffer[0]) {
        sprites[buffer[0]] = *(sprite_t *)buffer;
    }
}
void tx_event() {
    twi_transmit((uint8_t *)&sprites[role], sizeof(sprite_t));
    sprites[role].playerHit = 0xff;
}

void setup() {
    arduboy.boot();
    arduboy.safeMode();
    arduboy.setFrameRate(60);
    sincospi(orientation, &dirX, &dirY);

    twi_init();
    role = twi_handshake(targetAddresses, numPlayers);

    sprites[role].role = role;
    sprites[role].playerHit = 0xff;
    // TODO: add more spawning locations
    sprites[role].posX = controllerStartX;
    sprites[role].posY = controllerStartY;

    if (role > TWI_CONTROLLER) {
        twi_setGeneralCall(true);
        twi_attachSlaveRxEvent(rx_event);
        twi_attachSlaveTxEvent(tx_event);
    }
}

void loop() {
#ifdef FPS_DEBUG
    if (!Arduboy2Ex::nextFrameMiniDEV()) {
        return;
    }
#else
    if (!arduboy.nextFrame()) {
        return;
    }
#endif

    arduboy.pollButtons();
    if (arduboy.pressed(UP_BUTTON)) {
        momX += MUL32(dirX, moveSpeed) >> 16;   // Q16
        momY += MUL32(dirY, moveSpeed) >> 16;   // Q16
    }
    if (arduboy.pressed(DOWN_BUTTON)) {
        momX -= MUL32(dirX, moveSpeed) >> 16;   // Q16
        momY -= MUL32(dirY, moveSpeed) >> 16;   // Q16
    }
    if (arduboy.pressed(B_BUTTON)) {
        if (arduboy.pressed(RIGHT_BUTTON)) {
            momX += MUL32(dirY, moveSpeed) >> 16;   // Q16
            momY -= MUL32(dirX, moveSpeed) >> 16;   // Q16
        }
        if (arduboy.pressed(LEFT_BUTTON)) {
            momX -= MUL32(dirY, moveSpeed) >> 16;   // Q16
            momY += MUL32(dirX, moveSpeed) >> 16;   // Q16
        }
    } else {
        if (arduboy.pressed(RIGHT_BUTTON)) {
            orientation += turnSpeed;
            sincospi(orientation, &dirX, &dirY);    // Q15
        }
        if (arduboy.pressed(LEFT_BUTTON)) {
            orientation -= turnSpeed;
            sincospi(orientation, &dirX, &dirY);    // Q15
        }
    }
    
    bool firing = arduboy.justPressed(A_BUTTON);

    int8_t dX = (momX + 128) >> 8;  // Q8
    int8_t dY = (momY + 128) >> 8;  // Q8
    //_RPT1(_CRT_WARN, "%3d, %3d\n", dX, dY);

    // move, if not blocked by a wall
    uint8_t x0 = (sprites[role].posX + dX) >> 8;
    uint8_t y0 = (sprites[role].posY +  0) >> 8;
    uint8_t x1 = (sprites[role].posX +  0) >> 8;
    uint8_t y1 = (sprites[role].posY + dY) >> 8;

    if (MAP_LOOKUP(x0 * mapHeight + y0) == 0) sprites[role].posX += dX;
    if (MAP_LOOKUP(x1 * mapHeight + y1) == 0) sprites[role].posY += dY;

    // apply friction
    momX = MUL32(momX, friction) >> 8;
    momY = MUL32(momY, friction) >> 8;

    // vector perpendicular to dir, with length planeLen
    int16_t planeX = MUL32(dirY, planeLen) >> 15;    // Q15
    int16_t planeY = MUL32(-dirX, planeLen) >> 15;   // Q15

    // send and recieve over I2C
    if (role == TWI_CONTROLLER) {
        for (uint8_t i = 1; i < numPlayers; i++) {
            // get data from target
            twi_readFrom(pgm_read_byte(&targetAddresses[i]), (uint8_t *)&sprites[i], sizeof(sprite_t), true);

            // broadcast it via general call
            twi_writeTo(0x00, (uint8_t *)&sprites[i], sizeof(sprite_t), false, true);
        }
        twi_writeTo(0x00, (uint8_t *)&sprites[TWI_CONTROLLER], sizeof(sprite_t), false, true);
        sprites[TWI_CONTROLLER].playerHit = 0xff;
    }
    
    // pre-rendering math

    //which box of the map we're in
    int8_t mapX = sprites[role].posX >> 8;
    int8_t mapY = sprites[role].posY >> 8;

    uint16_t otherPlayerMapIndexes[numPlayers];

    bool hit = false;
    for (uint8_t i = 0; i < numPlayers; i++) {
        otherPlayerMapIndexes[i] = (sprites[i].posX >> 8) * mapHeight + (sprites[i].posY >> 8);
        hit |= (sprites[i].playerHit == role);
    }
    arduboy.fillScreen(hit ? WHITE : BLACK);

    for (uint8_t x = 0; x < FBW; x++) {

        static_assert(FBW == 128);
        int8_t cameraX = (x << 1) - 0x80; // (2 * x / 128 - 1) in Q7

        //calculate ray position and direction
        int16_t rayDirX = (dirX >> 1) + (MUL32(planeX, cameraX) >> 8); // Q14
        int16_t rayDirY = (dirY >> 1) + (MUL32(planeY, cameraX) >> 8); // Q14

        //length of ray from one x or y-side to next x or y-side
        uint16_t deltaDistX = recip(abs(rayDirX) >> 5);  // Q7
        uint16_t deltaDistY = recip(abs(rayDirY) >> 5);  // Q7
        deltaDistX = tmin<uint16_t>(deltaDistX, 0x7fff);
        deltaDistY = tmin<uint16_t>(deltaDistY, 0x7fff);

        //what direction to step in x or y-direction (either +1 or -1)
        int8_t stepX;
        int8_t stepY;

        uint16_t fractX; // Q8 [0, 256]
        uint16_t fractY; // Q8 [0, 256]

        //calculate step and initial sideDist
        if (rayDirX < 0) {
            stepX = -mapHeight;
            fractX = sprites[role].posX - (mapX << 8);   // Q8
        } else {
            stepX = +mapHeight;
            fractX = (1 << 8) - (sprites[role].posX - (mapX << 8));  // Q8
        }
        if (rayDirY < 0) {
            stepY = -1;
            fractY = sprites[role].posY - (mapY << 8);   // Q8
        } else {
            stepY = +1;
            fractY = (1 << 8) - (sprites[role].posY - (mapY << 8));  // Q8
        }

        //length of ray from current position to next x or y-side
        uint16_t sideDistX = UMUL32(fractX, deltaDistX) >> 8; // Q7
        uint16_t sideDistY = UMUL32(fractY, deltaDistY) >> 8; // Q7

        //perform DDA
        int8_t side = 0;    //was a NS or a EW wall hit?
        uint16_t mapIndex = mapX * mapHeight + mapY;  // [mapX][mapY] as 1-D index
        do {
            // check if laser beam has hit other player
            if (firing && x > 60 && x < 68) {
                // loop through all players
                for (uint8_t i = 0; i < numPlayers; i++) {
                    if (i != role && otherPlayerMapIndexes[i] == mapIndex) {
                        sprites[role].playerHit = i;
                        break;
                    }
                }
            }
            //jump to next map square, OR in x-direction, OR in y-direction
            if (sideDistX < sideDistY) {
                //if (sideDistX + deltaDistX >= (1<<16)) __debugbreak();
                sideDistX += deltaDistX;
                mapIndex += stepX;
                side = 0;
            } else {
                //if (sideDistY + deltaDistY >= (1<<16)) __debugbreak();
                sideDistY += deltaDistY;
                mapIndex += stepY;
                side = 1;
            }

        } while (MAP_LOOKUP(mapIndex) == 0);  //stop when ray has hit a wall

        uint8_t t = MAP_LOOKUP(mapIndex);

        //Calculate distance projected on camera direction (oblique distance will give fisheye effect!)
        uint16_t perpWallDist = (side == 0) ? sideDistX - deltaDistX : sideDistY - deltaDistY;  // Q7

        //save distance, for sprite clipping
        zbuf[x] = perpWallDist << 1;    // Q8

        uint16_t invPerpWallDist = recip(perpWallDist);
        //Calculate height of line to draw on screen
        //int8_t lineHeight = (perpWallDist > (1 << 7)) ? (FBH << 7) / perpWallDist : FBH;  // refcode
        //int8_t lineHeight = (perpWallDist > (1 << 7)) ? recip(perpWallDist) >> 3 : FBH;   // optimized for wallHeight=64
        uint16_t wallHeight = 64 * t;  // wallHeight can be arbitrary
        int8_t lineHeight;
        int8_t halfLineHeight64;
        if (perpWallDist > (1 << 7)) {
            lineHeight = UMUL32(wallHeight, invPerpWallDist) >> 9;
            halfLineHeight64 = invPerpWallDist >> 3 >> 1;
        } else {
            lineHeight = FBH;
            halfLineHeight64 = FBH / 2;
        }
        int8_t drawStart = (FBH / 2) - (lineHeight - halfLineHeight64);
        int8_t drawEnd = (FBH / 2) + halfLineHeight64;
        if (drawStart < 0) drawStart = 0;
        if (drawEnd >= FBH) drawEnd = FBH - 1;

        uint8_t c = t - 1;  //wall color = 0..3
        c = (c + side) % 4; //give x and y sides different brightness
        
        draw_vline(x, drawStart, drawEnd, PATTERNS[c]);
    }

    //
    //
    //
    //
    //
    // render sprites
    //
    //
    //
    //
    for (uint8_t s = 0; s < numSprites; s++) {
        if (s != role) {// don't draw ourself!
            // translate sprite position to relative to camera
            int16_t spriteX = sprites[s].posX - sprites[role].posX;
            int16_t spriteY = sprites[s].posY - sprites[role].posY;

            //transform sprite with the inverse camera matrix
            // [ planeX   dirX ] -1                                       [ dirY      -dirX ]
            // [               ]       =  1/(planeX*dirY-dirX*planeY) *   [                 ]
            // [ planeY   dirY ]                                          [ -planeY  planeX ]

            int16_t transformY = (MUL32(-planeY, spriteX) + MUL32(planeX, spriteY)) >> 15;  // Q8
            transformY = MUL32(transformY, invPlaneLen) >> 14;  // Q8

            if (transformY >= 16) { // cull if sprite is too close or behind

                int16_t transformX = (MUL32(dirY, spriteX) - MUL32(dirX, spriteY)) >> 15;   // Q8
                transformX = MUL32(transformX, invPlaneLen) >> 14;  // Q8

                // int spriteScreenX = int((FBW / 2) * (1.0 + transformX / transformY));
                int16_t invTransformY = recip(transformY); // [0, 16.0] in Q8
                int32_t scale32 = FIX16(1.0f, 8) + (MUL32(transformX, invTransformY) >> 8); // Q8
                int16_t scale = tclamp<int32_t>(scale32, -32768, 32767);  // Q8
                int16_t spriteScreenX = MUL32(FBW / 2, scale) >> 8;  // [-8192, 8191] in Q0

                // calculate width and height of the sprite on screen, using 'transformY' as distance to prevent fisheye
                int16_t spriteSize = MUL32(FBH, invTransformY) >> 8; // [0, 1023] in Q0

                uint8_t drawStartX = (uint8_t)tclamp<int16_t>(spriteScreenX - (spriteSize / 2), 0, 255);
                uint8_t drawEndX = (uint8_t)tclamp<int16_t>(spriteScreenX + (spriteSize / 2), 0, FBW);

                if (drawStartX < drawEndX) {    // cull if sprite is off the side

                    uint8_t drawStartY = (uint8_t)tclamp<int16_t>((FBH / 2) - (spriteSize / 2), 0, 255);
                    uint8_t drawEndY = (uint8_t)tclamp<int16_t>((FBH / 2) + (spriteSize / 2), 0, FBH);

                    uint16_t invSpriteSize = recip(spriteSize) - 1;   // ensure <= (65535/spriteSize)
                    uint16_t texStep = invSpriteSize / (256 / texSize);  // Q8

                    uint16_t texInitX = (2*drawStartX - 2*spriteScreenX + spriteSize) * (texSize/2);
                    uint16_t texInitY = (2*drawStartY - FBH + spriteSize) * (texSize/2);
                    texInitX = MUL32(texInitX, invSpriteSize) >> 8; // Q8
                    texInitY = MUL32(texInitY, invSpriteSize) >> 8; // Q8

                    uint16_t texX = texInitX;   // Q8
                    uint8_t drawStartByte = (((drawStartY >> 1) >> 1) >> 1); //right shift 3 without loop
                    uint8_t drawEndByte = (((drawEndY >> 1) >> 1) >> 1);  //The byte to end the unrolled loop on. Could be inclusive or exclusive

                    uint8_t preshift = texInitY >> 8;   //drawData.texYInit.getInteger();
                    uint8_t accumStart = texInitY & 0xff;   //drawData.texYInit.getFraction();
                    uint8_t fullStep = texStep >> 8;    //drawData.stepY.getInteger();
                    uint8_t fracStep = texStep & 0xff;  //drawData.stepY.getFraction();

                    for (uint8_t x = drawStartX; x < drawEndX; x++) { //For every strip (x)

                        //If the sprite is hidden, skip this line. Lots of calculations bypassed!
                        if ((uint16_t)transformY < zbuf[x]) {
                            zbuf[x] = transformY;

                            uint8_t tx = texX >> 8; //texX.getInteger();

                            //texData = readTextureStrip16(spritesheet, fr, tx) >> preshift;
                            //texMask = readTextureStrip16(spritesheet_Mask, fr, tx) >> preshift;
                            uint16_t texData = (_texData[tx] | (_texData[tx+16] << 8)) >> preshift;
                            uint16_t texMask = (_texMask[tx] | (_texMask[tx+16] << 8)) >> preshift;
                            if (texMask) {

                                uint8_t accum = accumStart;
                                uint8_t thisWallByte = drawStartByte;
                                uint16_t bofs = thisWallByte * FBW + x;
                                uint8_t texByte = buf[bofs];

                                // first and last bytes are tricky
                                if (drawStartY & 0x7) {
                                    uint8_t endFirst = tmin<uint8_t>((drawStartByte + 1) * 8, drawEndY);
                                    uint8_t bm = SET_MASK[drawStartY & 0x7];

                                    for (uint8_t i = drawStartY; i < endFirst; i++) {
                                        SPRITEBITUNROLL(bm, ~bm, texByte, texMask, texData, accum, fracStep, fullStep);
                                        bm <<= 1;
                                    }

                                    buf[bofs] = texByte;
                                    // texByte = buf[bofs += FBW];
                                    // ++thisWallByte;
                                    bofs = ++thisWallByte * FBW + x;
                                    texByte = buf[bofs];
                                }

                                // unrolled loop
                                for (; thisWallByte < drawEndByte; thisWallByte++) {
                                    SPRITEBITUNROLL(0b00000001, 0b11111110, texByte, texMask, texData, accum, fracStep, fullStep);
                                    SPRITEBITUNROLL(0b00000010, 0b11111101, texByte, texMask, texData, accum, fracStep, fullStep);
                                    SPRITEBITUNROLL(0b00000100, 0b11111011, texByte, texMask, texData, accum, fracStep, fullStep);
                                    SPRITEBITUNROLL(0b00001000, 0b11110111, texByte, texMask, texData, accum, fracStep, fullStep);
                                    SPRITEBITUNROLL(0b00010000, 0b11101111, texByte, texMask, texData, accum, fracStep, fullStep);
                                    SPRITEBITUNROLL(0b00100000, 0b11011111, texByte, texMask, texData, accum, fracStep, fullStep);
                                    SPRITEBITUNROLL(0b01000000, 0b10111111, texByte, texMask, texData, accum, fracStep, fullStep);
                                    SPRITEBITUNROLL(0b10000000, 0b01111111, texByte, texMask, texData, accum, fracStep, fullStep);

                                    buf[bofs] = texByte;
                                    texByte = buf[bofs += FBW];
                                }

                                //Last byte, but only need to do it if we end in the middle of a byte and don't simply span one byte
                                if ((drawEndY & 0x7) && drawStartByte != drawEndByte) {
                                    uint8_t endStart = thisWallByte * 8;
                                    uint8_t bm = SET_MASK[0];

                                    for (uint8_t i = endStart; i < drawEndY; i++) {
                                        SPRITEBITUNROLL(bm, ~bm, texByte, texMask, texData, accum, fracStep, fullStep);
                                        bm <<= 1;
                                    }

                                    buf[bofs] = texByte;
                                }
                            }
                        }
                        texX += texStep;
                    }            
                }
            }
        }
    }
    if (firing) {
        Sprites::drawPlusMask(laserTaggerX + laserTaggerFlashOffsetX, laserTaggerY + laserTaggerFlashOffsetY, laserTaggerFlashPlusMask, 0);
    }
    Sprites::drawPlusMask(laserTaggerX, laserTaggerY, laserTaggerPlusMask, 0);
    arduboy.display();
}
