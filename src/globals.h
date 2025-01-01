#pragma once
#include <Arduboy2.h>

#define I2C_MAX_PLAYERS 4
#define I2C_FREQUENCY 400000

#include <ArduboyI2C.h>
#include "Font3x5.h"

// #define FPS_DEBUG
#define AUTO_START false

using uint24_t = __uint24;

enum State {
    TITLE,
    SETTINGS_INIT,
    SETTINGS,
    CREDITS_INIT,
    CREDITS,
    LOBBY_INIT,
    LOBBY,
    GAME_INIT,
    GAME
};

struct startpos_t {
	int16_t x;
	int16_t y;
	uint16_t orientation;
};

struct sprite_t {
    int16_t posX;  // Q8
    int16_t posY;  // Q8
    char name[ARDUBOY_UNIT_NAME_BUFFER_SIZE];
    uint8_t otherPlayerHit;
    uint8_t timeout;
    uint8_t health;
    uint8_t id; // must be last
};


#ifndef __AVR__
#define PSTR(str_) str_
#define PROGMEM
inline uint8_t pgm_read_byte(void const* p) { return *(uint8_t*)p; }
inline uint16_t pgm_read_word(void const* p) { return *(uint16_t*)p; }
inline void const* pgm_read_ptr(void const* p) { return *(void const**)p; }
#endif

// high/low byte of uint16_t
#define LO(a) ((uint8_t)(a))
#define HI(a) ((uint8_t)((uint16_t)(a) >> 8))

// perform a 1-D lookup into worldMap[]
#define MAP_LOOKUP(i) (pgm_read_byte((uint8_t *)worldMap + i))

#define _STR(str) #str
#define STR(str) _STR(str)

#define CENTER_STR(str) WIDTH / 2 - ((sizeof(str) - 1) * (FONT3X5_WIDTH + 1) - 1) / 2
#define CENTER_WIDTH(width) WIDTH / 2 - width / 2
#ifdef _MSC_VER
#define FORCEINLINE __forceinline
#else
#define FORCEINLINE __attribute__((always_inline)) inline
#endif

#ifdef __AVR__
FORCEINLINE uint8_t CLZ16(uint16_t u) { return __builtin_clz(u); }
#else
FORCEINLINE uint8_t CLZ16(uint16_t a) {
    uint8_t r = 16;
    if (a >= 0x0100) { a >>= 8; r -= 8; }
    if (a >= 0x0010) { a >>= 4; r -= 4; }
    if (a >= 0x0004) { a >>= 2; r -= 2; }
    r -= a - (a & (a >> 1));
    return r;
}
#endif

// convert float to signed 16-bit fixed-point, with q fraction bits
#define FIX16(x, q) ((int16_t)((float)(x) * (1UL << (q)) + ((x) < 0.0f ? -0.5f : 0.5f)))

// convert float to unsigned 16-bit fixed-point, with q fraction bits
#define UFIX16(x, q) ((uint16_t)((float)(x) * (1UL << (q)) + 0.5f))

// signed 16x16 multiply, with 32-bit result
#define MUL32(a, b)   ((int32_t)(int16_t)(a) * (int32_t)(int16_t)(b))

// unsigned 16x16 multiply, with 32-bit result
#define UMUL32(a, b)  ((uint32_t)(uint16_t)(a) * (uint32_t)(uint16_t)(b))

#ifdef __AVR__
FORCEINLINE int16_t MUL15(int16_t a, int16_t b) {
    /*
               A1 A0
               B1 B0
            ========
               A0*B0
            A1*B0
            A0*B1
         A1*B1
         ===========
         R1 R0
    */
    int16_t r;
    int8_t t;
    int8_t z = 0;
    asm volatile(
        "fmuls  %B[a], %B[b]      \n\t" // A1*B1
        "movw   %A[r], r0         \n\t"

        "fmul   %A[a], %A[b]      \n\t" // A0*B0
        "adc    %A[r], %[z]       \n\t"
        "mov    %[t] , r1         \n\t"

        "fmulsu %B[a], %A[b]      \n\t" // A1*B0
        "sbc    %B[r], %[z]       \n\t"
        "add    %[t] , r0         \n\t"
        "adc    %A[r], r1         \n\t"
        "adc    %B[r], %[z]       \n\t"

        "fmulsu %B[b], %A[a]      \n\t" // B1*A0
        "sbc    %B[r], %[z]       \n\t"
        "add    %[t] , r0         \n\t"
        "adc    %A[r], r1         \n\t"
        "adc    %B[r], %[z]       \n\t"
        "clr    r1                \n\t"
        : [r] "=&r" (r),
          [t] "=&r" (t)
        : [a] "a"   (a),
          [b] "a"   (b),
          [z] "r"   (z)
        :
    );
    return r;
}
#else
#define MUL15(a, b) (int16_t)(((int32_t)(int16_t)a * (int32_t)(int16_t)b) >> 15)
#endif

#ifdef __AVR__
FORCEINLINE void SPRITEBITUNROLL(uint8_t bm, uint8_t nbm,
                                 uint8_t& texByte, uint16_t& texMask, uint16_t& texData,
                                 uint8_t& accum, uint8_t fracStep, uint8_t fullStep) {

    if (texMask & 1) { if (texData & 1) texByte |= bm; else texByte &= nbm; }
    asm volatile(
        "add %[accum], %[step]  \n"
        "brcc .+8               \n"
        "lsr %B[tdata]          \n"
        "ror %A[tdata]          \n"
        "lsr %B[tmask]          \n"
        "ror %A[tmask]          \n"
        : [accum] "+&r" (accum),
          [tdata] "+&r" (texData),
          [tmask] "+&r" (texMask)
        : [step] "r" (fracStep)
    );
    if (fullStep) { texMask >>= fullStep; texData >>= fullStep; }
}
#else
FORCEINLINE void SPRITEBITUNROLL(uint8_t bm, uint8_t nbm,
                                 uint8_t& texByte, uint16_t& texMask, uint16_t& texData,
                                 uint8_t& accum, uint8_t fracStep, uint8_t fullStep) {

    if (texMask & 1) { if (texData & 1) texByte |= bm; else texByte &= nbm; }

    uint8_t lastAccum = accum;
    accum += fracStep;
    if (accum < lastAccum) { texMask >>= 1; texData >>= 1; }

    if (fullStep) { texMask >>= fullStep; texData >>= fullStep; }
}
#endif

extern const uint16_t recip_tab[128] PROGMEM;

// for x!=0 returns 0xffff / x
// for x==0 returns 0xffff
// max error = 1ULP
FORCEINLINE uint16_t recip(uint16_t x) {
    if (x == 0) return 0xffff;

    uint8_t n = CLZ16(x);
    x <<= n;                                    // normalize, so MSB is 1
    uint16_t r = pgm_read_word(&recip_tab[(x >> 8) - 0x80]);    // r = 0x7fffff / x, where x = 8 MSBs
    r >>= 15 - n;                               // undo normalize
    return r;
}

template<class T> FORCEINLINE T tmin(T a, T b) { return a < b ? a : b; }
template<class T> FORCEINLINE T tmax(T a, T b) { return a < b ? b : a; }
template<class T> FORCEINLINE T tclamp(T x, T a, T b) { return tmin(tmax(x, a), b); }
template<class T> FORCEINLINE T tsign(T a, T scale) { return a < 0 ? -scale : a > 0 ? scale : 0; }

void display_fill_screen(uint8_t color);
uint16_t rsqrt(uint16_t x);
void sincospi(uint16_t ux, int16_t* ps, int16_t* pc);
void initFastRandomSeed();
uint16_t fastRandom();

constexpr int FBW = 128;
constexpr int FBH = 64;
constexpr int BUF_BYTES = FBW * FBH / 8;

constexpr int16_t planeLen = FIX16(0.66f, 15);  // Q15, sets FOV
constexpr int16_t invPlaneLen = FIX16(1/0.66f, 14);  // Q14
constexpr int16_t moveSpeed = FIX16(0.005f, 17); // Q17
constexpr int16_t turnSpeed = FIX16(0.01f, 15); // Q15
constexpr int16_t friction = FIX16(0.9f, 8);    // Q8
constexpr uint8_t maxHealth = 20;

constexpr uint8_t mapWidth = 64;
constexpr uint8_t mapHeight = 64;

const uint8_t numDoors = 15;
const uint8_t secretDoor = 5;

constexpr uint8_t laserTaggerWidth = 50;
constexpr uint8_t laserTaggerHeight = 32;

constexpr int8_t laserTaggerOffsetX = 12;

constexpr uint8_t laserTaggerX = CENTER_WIDTH(laserTaggerWidth) + laserTaggerOffsetX;
constexpr uint8_t laserTaggerY = HEIGHT - laserTaggerHeight;

constexpr uint8_t laserTaggerFlashWidth = 29;
constexpr uint8_t laserTaggerFlashHeight = 29;

constexpr int8_t laserTaggerFlashOffsetX = 0;
constexpr int8_t laserTaggerFlashOffsetY = -5;

constexpr uint8_t texSize = 16;    // should be even
constexpr uint8_t texBytes = texSize * texSize / 8;

constexpr uint8_t titleImageWidth = 72;
constexpr uint8_t titleImageHeight = 47;

constexpr uint8_t menuImageWidth = 5;
constexpr uint8_t menuImageHeight = 5;

constexpr uint8_t nameTempBuffer = 0;

constexpr uint8_t nullId = 255;

constexpr uint8_t numItems = 10;

constexpr uint8_t singleplayerId = 0;

extern Arduboy2Base arduboy;
extern Font3x5 font3x5;

extern State state;

extern uint8_t id;

extern uint8_t health;

extern uint16_t orientation;    // brad [0,2] in Q15

// (dirX, dirY) is the unit vector derived from orientation
// initialized in setup
extern int16_t dirX;   // Q15
extern int16_t dirY;   // Q15

extern int16_t momX;   // Q16
extern int16_t momY;   // Q16

extern uint16_t zbuf[FBW]; // Q8

extern uint8_t doors[numDoors];

// public player data / other player data
extern volatile sprite_t sprites[I2C_MAX_PLAYERS];

extern bool singleplayer;

extern int8_t menuIdx;
extern uint8_t usernameIdx;
extern int8_t creditsY;

extern uint16_t seed;

extern const uint8_t PROGMEM worldMap[mapWidth][mapHeight] ;

extern const unsigned char PROGMEM laserTaggerPlusMask[];
extern const unsigned char PROGMEM laserTaggerFlashPlusMask[];
extern const uint8_t PROGMEM _texData[];
extern const uint8_t PROGMEM _texMask[];
extern const uint8_t PROGMEM titleImage[];
extern const uint8_t PROGMEM menuImage[];

extern const startpos_t startPos[I2C_MAX_PLAYERS] PROGMEM;

void draw_vline(uint8_t x, int16_t y0, int16_t y1, uint16_t pat);
void raycast(sprite_t *sprite, uint16_t *perpWallDist, uint8_t *t, uint8_t *side, int16_t rayDirX, int16_t rayDirY, bool shoot);
void render();

void reset_ai(sprite_t *sprite);
void reset_ais();
void update_ais();

void setup_lobby();
uint8_t run_timeout();
void run_lobby(uint8_t numPlayers);
void update_multiplayer();


void reset_player();
void move_player();
bool handle_player_hit();
void move_sprite(sprite_t *sprite, int8_t dX, int8_t dY);


void draw_health_bar();
void update_title_screen();
void draw_title_screen();
void init_settings_screen();
void update_settings_screen();
void draw_settings_screen();
void init_credits_screen();
void update_credits_screen();
void draw_credits_screen();