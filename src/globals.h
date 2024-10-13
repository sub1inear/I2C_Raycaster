#pragma once
#include <Arduboy2.h>

#define I2C_MAX_PLAYERS 4
#define I2C_FREQUENCY 400000

#include <ArduboyI2C.h>
#include "Font3x5.h"
#include "player.h"
#include "render.h"
#include "maps.h"
#include "images.h"
#include "multiplayer.h"

struct sprite_t {
    int16_t posX;  // Q8
    int16_t posY;  // Q8
    char name[ARDUBOY_UNIT_NAME_BUFFER_SIZE];
    uint8_t otherPlayerHit;
    uint8_t id;
};

constexpr int FBW = 128;
constexpr int FBH = 64;
constexpr int BUF_BYTES = FBW * FBH / 8;

constexpr int16_t planeLen = FIX16(0.66f, 15);  // Q15, sets FOV
constexpr int16_t invPlaneLen = FIX16(1/0.66f, 14);  // Q14
constexpr int16_t moveSpeed = FIX16(0.005f, 17); // Q17
constexpr int16_t turnSpeed = FIX16(0.01f, 15); // Q15
constexpr int16_t friction = FIX16(0.9f, 8);    // Q8

extern Arduboy2Base arduboy;
extern Font3x5 font3x5;

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

// public player data / other player data
extern volatile sprite_t sprites[I2C_MAX_PLAYERS];