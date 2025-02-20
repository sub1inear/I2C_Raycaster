#include "globals.h"

void reset_ai(sprite_t *ai) {
    ai->posX = pgm_read_word(&playerStartPos[ai - sprites].posX);
    ai->posY = pgm_read_word(&playerStartPos[ai - sprites].posY);
    ai->timeout = 1;
    ai->health = aiMaxHealth;
}

void init_ai(sprite_t *ai) {
    ai->deaths = 0;
    ai->eliminations = 0;
    ai->otherPlayerHit = nullId;
    ai->eliminatedBy = nullId;
    ai->powerupTaken = nullId;
    ai->name[0] = 'A';
    ai->name[1] = 'I';
    ai->name[2] = '0' + ai - sprites;
    ai->name[3] = '\0';
    reset_ai(ai);
}

void init_ais() {
    for (uint8_t i = 1; i < I2C_MAX_PLAYERS; i++) {
        init_ai((sprite_t *)&sprites[i]);
    }
}

void update_ais() {
    sprite_t *player = (sprite_t *)&sprites[singleplayerId];
    for (uint8_t i = 1; i < I2C_MAX_PLAYERS; i++) {
        sprite_t *ai = (sprite_t *)&sprites[i];
        ai->otherPlayerHit = nullId;
        ai->eliminatedBy = nullId;

        int8_t dX = (player->posX - ai->posX) >> 8; // Q0
        int8_t dY = (player->posY - ai->posY) >> 8; // Q0

        uint16_t distSq = dX * dX + dY * dY; // Q0
        if (distSq < 20 * 20) {
            if ((fast_random() & 0x3f) == 0) {
                // normalize vectors
                uint16_t invDist = fixpt::rsqrt(distSq) >> 1; // Q15

                int16_t rayDirX = MUL32(dX, invDist) >> 1; // Q14
                int16_t rayDirY = MUL32(dY, invDist) >> 1; // Q14
                // save memory with union
                union {
                    uint16_t perpWallDist;
                    uint8_t t;
                    uint8_t side;
                };
                raycast(ai, &perpWallDist, &t, &side, rayDirX, rayDirY, true);
            }
        }
        if (distSq > 2 * 2) {
            dX = tsign(dX, (int8_t)8);
            dY = tsign(dY, (int8_t)8);
            
            move_sprite(ai, dX, dY);
        }
        
        if (player->otherPlayerHit == i) {
            ai->health--;
            if (ai->health == 0) {
                reset_ai(ai);
                if (ai->deaths < 255)
                    ai->deaths++;
                ai->eliminatedBy = id;
            }
        }
        if (player->eliminatedBy == i)
            if (ai->eliminations < 255)
                ai->eliminations++;
    }
}