#include "globals.h"

void move_sprite(sprite_t *sprite, int8_t dX, int8_t dY) {
    // move, if not blocked by a wall
    uint8_t x0 = (sprite->posX + dX) >> 8;
    uint8_t y0 = (sprite->posY +  0) >> 8;
    uint8_t x1 = (sprite->posX +  0) >> 8;
    uint8_t y1 = (sprite->posY + dY) >> 8;

    uint8_t t1 = MAP_LOOKUP(x0 * mapHeight + y0);
    uint8_t t2 = MAP_LOOKUP(x1 * mapHeight + y1);
    
    if (t1 == 0 || (t1 >= secretDoor && doors[t1 >> 4])) sprite->posX += dX;
    if (t2 == 0 || (t2 >= secretDoor && doors[t2 >> 4])) sprite->posY += dY;
}
