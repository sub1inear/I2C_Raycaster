#include "ai.h"
#include "globals.h"
#include "utils.h"

void reset_ais() {
    for (uint8_t i = 1; i < I2C_MAX_PLAYERS; i++) {
        sprite_t *ai = (sprite_t *)&sprites[i];
        ai->posX = pgm_read_word(&startPos[i].x);
        ai->posY = pgm_read_word(&startPos[i].y);
        ai->timeout = 1;
    }
}

void update_ais() {
    sprite_t *player = (sprite_t *)&sprites[singleplayerId];
    for (uint8_t i = 1; i < I2C_MAX_PLAYERS; i++) {
        sprite_t *ai = (sprite_t *)&sprites[i];

        int8_t dX = (player->posX - ai->posX) >> 8;
        int8_t dY = (player->posY - ai->posY) >> 8;
        
        uint16_t distsq = dX * dX + dY * dY;
        if (distsq < 10 * 10) {
        } else {
            dX = tsign(dX) << 3;
            dY = tsign(dY) << 3;
            
            move_sprite(ai, dX, dY);
        }
    }
}