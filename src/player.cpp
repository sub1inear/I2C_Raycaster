#include "globals.h"

void reset_player() {
    sprite_t *player = (sprite_t *)&sprites[id];
    maxHealth = initialMaxHealth;
    moveSpeed = initialMoveSpeed;
    shield = 0;
    player->health = maxHealth;
    player->posX = pgm_read_word(&playerStartPos[id].posX);
    player->posY = pgm_read_word(&playerStartPos[id].posY);
    momX = 0;
    momY = 0;
    orientation = pgm_read_word(&playerStartPos[id].orientation);  
    fixpt::sincospi(orientation, dirX, dirY);
}

bool check_username_empty() {
    sprite_t *player = (sprite_t *)&sprites[id];
    for (char *p = player->name; *p != '\0'; p++) {
        if (*p != ' ')
            return false;
    }
    return true;
}

void init_player() {
    sprite_t *player = (sprite_t *)&sprites[id];
    leaveTimer = 0;
    player->eliminations = 0;
    player->deaths = 0;
    // need init because set after first update multiplayer
    // not needed with player->powerupTaken
    player->otherPlayerHit = nullId;
    player->eliminatedBy = nullId;
    if (check_username_empty())
        strcpy_P((char *)player->name, PSTR("Player"));
    reset_player();
}

void update_player() {
    sprite_t *player = (sprite_t *)&sprites[id];
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
            fixpt::sincospi(orientation, dirX, dirY);    // Q15
        }
        if (arduboy.pressed(LEFT_BUTTON)) {
            orientation -= turnSpeed;
            fixpt::sincospi(orientation, dirX, dirY);    // Q15
        }
    }
    if (arduboy.pressed(A_BUTTON | B_BUTTON)) {
        leaveTimer++;
        if (leaveTimer > leaveTimerMax) {
            stop_multiplayer();
            state = TITLE;
        }    
    } else
        leaveTimer = 0;
    
    int8_t dX = (momX + 128) >> 8;  // Q8
    int8_t dY = (momY + 128) >> 8;  // Q8

    move_sprite(player, dX, dY);

    // apply friction
    momX = MUL32(momX, friction) >> 8;
    momY = MUL32(momY, friction) >> 8;
}