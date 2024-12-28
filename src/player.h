#pragma once
#include <Arduboy2.h>

struct startpos_t {
	int16_t x;
	int16_t y;
	uint16_t orientation;
};

void reset_player();
void move_player();
bool handle_player_hit();

