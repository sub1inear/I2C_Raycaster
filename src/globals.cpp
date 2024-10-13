#include "globals.h"

Arduboy2Base arduboy;
Font3x5 font3x5;

// private player data

uint8_t id;

uint8_t health;

uint16_t orientation;    // brad [0,2] in Q15

// (dirX, dirY) is the unit vector derived from orientation
// initialized in setup
int16_t dirX;   // Q15
int16_t dirY;   // Q15

int16_t momX;   // Q16
int16_t momY;   // Q16

uint16_t zbuf[FBW]; // Q8

// public player data / other player data
volatile sprite_t sprites[I2C_MAX_PLAYERS];

