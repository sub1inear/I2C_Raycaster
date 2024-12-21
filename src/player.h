#pragma once
#include <Arduboy2.h>

enum SpriteType : uint8_t {
    PLAYER,
    HEALTH_POWERUP,
    SHIELD_POWERUP,
    SPEED_POWERUP,
};

struct sprite_t {
    int16_t posX;  // Q8
    int16_t posY;  // Q8
    char name[ARDUBOY_UNIT_NAME_BUFFER_SIZE];
    uint8_t otherPlayerHit;
    uint8_t timeout;
    SpriteType type;
    uint8_t id; // must be last
};

struct startpos_t {
	int16_t x;
	int16_t y;
	uint16_t orientation;
};

void reset_player();
void move_player();
bool handle_player_hit();