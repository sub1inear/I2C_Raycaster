#include "player.h"
#include "globals.h"
#include "utils.h"

void reset_player() {
    health = 20;
    sprites[id].posX = pgm_read_word(&startPos[id].x);
    sprites[id].posY = pgm_read_word(&startPos[id].y);
    orientation = pgm_read_word(&startPos[id].orientation);    
    sincospi(orientation, &dirX, &dirY);
}

void move_player() {
    if (arduboy.pressed(UP_BUTTON)) {
        momX += MUL32(dirX, moveSpeed) >> 16;   // Q16
        momY += MUL32(dirY, moveSpeed) >> 16;   // Q16
    }
    if (arduboy.pressed(DOWN_BUTTON)) {
        momX -= MUL32(dirX, moveSpeed) >> 16;   // Q16
        momY -= MUL32(dirY, moveSpeed) >> 16;   // Q16
    }
    if (arduboy.pressed(B_BUTTON)) {
        if (arduboy.pressed(RIGHT_BUTTON)) {
            momX += MUL32(dirY, moveSpeed) >> 16;   // Q16
            momY -= MUL32(dirX, moveSpeed) >> 16;   // Q16
        }
        if (arduboy.pressed(LEFT_BUTTON)) {
            momX -= MUL32(dirY, moveSpeed) >> 16;   // Q16
            momY += MUL32(dirX, moveSpeed) >> 16;   // Q16
        }
    } else {
        if (arduboy.pressed(RIGHT_BUTTON)) {
            orientation += turnSpeed;
            sincospi(orientation, &dirX, &dirY);    // Q15
        }
        if (arduboy.pressed(LEFT_BUTTON)) {
            orientation -= turnSpeed;
            sincospi(orientation, &dirX, &dirY);    // Q15
        }
    }
    
    int8_t dX = (momX + 128) >> 8;  // Q8
    int8_t dY = (momY + 128) >> 8;  // Q8

    // move, if not blocked by a wall
    uint8_t x0 = (sprites[id].posX + dX) >> 8;
    uint8_t y0 = (sprites[id].posY +  0) >> 8;
    uint8_t x1 = (sprites[id].posX +  0) >> 8;
    uint8_t y1 = (sprites[id].posY + dY) >> 8;

    if (MAP_LOOKUP(x0 * mapHeight + y0) == 0) sprites[id].posX += dX;
    if (MAP_LOOKUP(x1 * mapHeight + y1) == 0) sprites[id].posY += dY;

    // apply friction
    momX = MUL32(momX, friction) >> 8;
    momY = MUL32(momY, friction) >> 8;
}

bool handle_player_hit() {
    for (uint8_t i = 0; i < I2C_MAX_PLAYERS; i++) {
        if (sprites[i].otherPlayerHit == id) {
            health--;
            if (health == 0) {
                reset_player();
            }
            return true;
        }
    }
    return false;
}