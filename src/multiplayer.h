#pragma once
#include "stdint.h"

void setup_lobby();
uint8_t run_timeout();
void run_lobby(uint8_t numPlayers);
void update_multiplayer();