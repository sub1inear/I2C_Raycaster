#include "globals.h"

void init_powerups() {
    for (uint8_t i = 0; i < numPowerups; i++) {
        sprite_t *powerup = (sprite_t *)&sprites[i + I2C_MAX_PLAYERS];
        powerup->posX = pgm_read_word(&powerupStartPos[i].posX);
        powerup->posY = pgm_read_word(&powerupStartPos[i].posY);
        powerup->type = pgm_read_byte(&powerupStartPos[i].type);
        powerup->timeout = 1;
    }
}
bool update_powerups() {
    sprite_t *player = (sprite_t *)&sprites[id];
    player->powerupTaken = nullId;

    for (uint8_t i = I2C_MAX_PLAYERS; i < I2C_MAX_PLAYERS + numPowerups; i++) {
        sprite_t *powerup = (sprite_t *)&sprites[i];
        if (powerup->timeout) {

            int8_t dX = (player->posX - powerup->posX) >> 8; // Q0
            int8_t dY = (player->posY - powerup->posY) >> 8; // Q0
            
            uint16_t distSq = dX * dX + dY * dY; // Q0
            if (distSq < 1 * 1) {
                player->powerupTaken = i;
                powerup->timeout = 0;
                switch (powerup->type) {
                case HEALTH:
                    maxHealth += 5;
                    health = maxHealth;
                    break;
                case SHIELD:
                    shield += 2;
                    break;
                case SPEED_BOOST:
                    if (moveSpeed < maxMoveSpeed)
                        moveSpeed += FIX16(0.005f, 17);
                    break;
                case PLAYER:
                    break;
                }
                return true;
            }
        }
    }
    return false;
}