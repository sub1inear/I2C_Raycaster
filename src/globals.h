#pragma once
#include <Arduboy2.h>

#define I2C_MAX_PLAYERS 4
#define I2C_CUSTOM_HANDSHAKE
#define I2C_FREQUENCY 400000

#include <ArduboyI2C.h>
#include "Font3x5.h"
#include "fixpt.h"

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
    GAME,
    GAME_OVER_INIT,
    GAME_OVER,
};

// 8 max sprite types because of image index overflow
enum SpriteType : uint8_t {
    PLAYER,
    HEALTH,
    SHIELD,
    SPEED_BOOST,
};

struct player_startpos_t {
	int16_t posX;
	int16_t posY;
	uint16_t orientation;
};

struct powerup_startpos_t {
    int16_t posX;
    int16_t posY;
    SpriteType type;
};

struct sprite_t {
    int16_t posX;  // Q8
    int16_t posY;  // Q8
    char name[ARDUBOY_UNIT_NAME_BUFFER_SIZE];
    uint8_t otherPlayerHit;
    uint8_t eliminatedBy;
    uint8_t eliminations;
    uint8_t deaths;
    uint8_t timeout;
    uint8_t health;
    uint8_t powerupTaken;
    SpriteType type;
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

#define STR_WIDTH(chars) ((chars - 1) * (FONT3X5_WIDTH + 1) - 1)
#define CENTER_WIDTH(width) (WIDTH / 2 - width / 2)
#define CENTER_STR(str) CENTER_WIDTH(STR_WIDTH(sizeof(str)))

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

template<class T> FORCEINLINE T tmin(T a, T b) { return a < b ? a : b; }
template<class T> FORCEINLINE T tmax(T a, T b) { return a < b ? b : a; }
template<class T> FORCEINLINE T tclamp(T x, T a, T b) { return tmin(tmax(x, a), b); }
template<class T> FORCEINLINE T tsign(T a, T scale) { return a < 0 ? -scale : a > 0 ? scale : 0; }

void display_fill_screen(bool fill, uint8_t color);
void wipe_effect();
void swap(uint8_t *array, uint8_t i, uint8_t j);
void init_fast_random_seed();
uint16_t fast_random();

constexpr int FBW = 128;
constexpr int FBH = 64;
constexpr int BUF_BYTES = FBW * FBH / 8;

constexpr int16_t planeLen = FIX16(0.66f, 15);  // Q15, sets FOV
constexpr int16_t invPlaneLen = FIX16(1/0.66f, 14);  // Q14
constexpr int16_t initialMoveSpeed = FIX16(0.005f, 17);  // Q17
constexpr int16_t maxMoveSpeed = FIX16(0.01f, 17);
constexpr int16_t turnSpeed = FIX16(0.01f, 15); // Q15
constexpr int16_t friction = FIX16(0.9f, 8);    // Q8
constexpr uint8_t initialMaxHealth = 20;
constexpr uint8_t aiMaxHealth = 10;

constexpr uint8_t mapWidth = 64;
constexpr uint8_t mapHeight = 64;

constexpr uint8_t numDoors = 15;
constexpr uint8_t secretDoor = 5;

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

constexpr uint8_t titleImageWidth = 70;
constexpr uint8_t titleImageHeight = 43;

constexpr uint8_t menuImageWidth = 5;
constexpr uint8_t menuImageHeight = 5;

constexpr uint8_t nullId = 255;

constexpr uint8_t numPowerups = 12;

constexpr uint8_t singleplayerId = 0;

constexpr uint32_t gameMillisMax = (uint16_t)1 * 60 * 1000;

constexpr uint8_t leaveTimerMax = 200;

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
extern volatile sprite_t sprites[I2C_MAX_PLAYERS + numPowerups];

extern bool singleplayer;

extern int8_t menuIdx;
extern uint8_t usernameIdx;
extern int8_t creditsY;

extern uint16_t seed;

extern uint32_t gameStart;

extern uint8_t leaveTimer;

extern int16_t moveSpeed;
extern uint8_t maxHealth;
extern uint8_t shield;

extern const uint8_t PROGMEM wipeEffectDelay[WIDTH];

extern const uint8_t PROGMEM worldMap[mapWidth][mapHeight];

extern const unsigned char PROGMEM laserTaggerPlusMask[];
extern const unsigned char PROGMEM laserTaggerFlashPlusMask[];
extern const uint8_t PROGMEM _texData[];
extern const uint8_t PROGMEM _texMask[];
extern const uint8_t PROGMEM titleImage[];
extern const uint8_t PROGMEM menuImage[];

extern const player_startpos_t playerStartPos[I2C_MAX_PLAYERS] PROGMEM;
extern const powerup_startpos_t powerupStartPos[numPowerups] PROGMEM;
extern const char numberSuffixes[][3] PROGMEM;

void draw_vline(uint8_t x, int16_t y0, int16_t y1, uint16_t pat);
void raycast(sprite_t *sprite, uint16_t *perpWallDist, uint8_t *t, uint8_t *side, int16_t rayDirX, int16_t rayDirY, bool shoot);
void render();

void reset_ai(sprite_t *sprite);
void init_ai(sprite_t *ai);
void init_ais();
void update_ais();

void on_receive();
void start_multiplayer();
void stop_multiplayer();
bool check_no_i2c();
void setup_lobby();
uint8_t run_timeout();
void run_lobby(uint8_t numPlayers);
void update_multiplayer();
bool receive_multiplayer();

void reset_player();
bool check_username_empty();
void init_player();
void update_player();

void move_sprite(sprite_t *sprite, int8_t dX, int8_t dY);

void init_powerups();
bool update_powerups();

void draw_health_bar();
void update_title_screen();
void draw_title_screen();
void init_settings_screen();
void update_settings_screen();
void draw_settings_screen();
void init_credits_screen();
void update_credits_screen();
void draw_credits_screen();
bool check_game_over();
void update_game_over();
void draw_game_over();