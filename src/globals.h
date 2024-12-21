#pragma once
#include <Arduboy2.h>

#define I2C_MAX_PLAYERS 4
#define I2C_FREQUENCY 400000

#include <ArduboyI2C.h>
#include "Font3x5.h"
#include "player.h"
#include "render.h"
#include "multiplayer.h"
#include "ui.h"
#include "utils.h"

// #define FPS_DEBUG
#define AUTO_START false

enum State {
    TITLE,
    SETTINGS_INIT,
    SETTINGS,
    CREDITS_INIT,
    CREDITS,
    LOBBY_INIT,
    LOBBY,
    SINGLEPLAYER_INIT,
    GAME
};


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

constexpr uint8_t titleImageWidth = 72;
constexpr uint8_t titleImageHeight = 47;

constexpr uint8_t menuImageWidth = 5;
constexpr uint8_t menuImageHeight = 5;

constexpr uint8_t nameTempBuffer = 0;

constexpr uint8_t nullId = 255;

constexpr uint8_t numItems = 10;

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

extern const uint8_t PROGMEM worldMap[mapWidth][mapHeight] ;

extern const unsigned char PROGMEM laserTaggerPlusMask[];
extern const unsigned char PROGMEM laserTaggerFlashPlusMask[];
extern const uint8_t PROGMEM _texData[];
extern const uint8_t PROGMEM _texMask[];
extern const uint8_t PROGMEM titleImage[];
extern const uint8_t PROGMEM menuImage[];