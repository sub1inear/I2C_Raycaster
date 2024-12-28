#pragma once
#include <Arduboy2.h>

struct sprite_t {
    int16_t posX;  // Q8
    int16_t posY;  // Q8
    char name[ARDUBOY_UNIT_NAME_BUFFER_SIZE];
    uint8_t otherPlayerHit;
    uint8_t timeout;
    uint8_t id; // must be last
};

void move_sprite(sprite_t *sprite, int8_t dX, int8_t dY);
