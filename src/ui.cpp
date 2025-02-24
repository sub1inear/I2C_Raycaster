#include "globals.h"

const State onClick[4] PROGMEM = {GAME_INIT, LOBBY_INIT, SETTINGS_INIT, CREDITS_INIT};

void draw_health_bar() {
}

void update_title_screen() {
    menuIdx += arduboy.justPressed(DOWN_BUTTON) - arduboy.justPressed(UP_BUTTON);
    menuIdx = tclamp(menuIdx, (int8_t)0, (int8_t)3);
    if (arduboy.justPressed(A_BUTTON)) {
        state = pgm_read_byte(&onClick[menuIdx]);
        singleplayer = state == GAME_INIT;
    }
}

void draw_title_screen() {
    Sprites::drawSelfMasked(CENTER_WIDTH(titleImageWidth), 0, titleImage, 0);
    font3x5.setCursor(CENTER_STR("Singleplayer"), 41);
    font3x5.print(F("Singleplayer\nMultiplayer\nSettings\nCredits"));
    Sprites::drawSelfMasked(35, 41 + menuIdx * FONT3X5_HEIGHT, menuImage, 0);
}

void init_settings_screen() {
    usernameIdx = 0;
    arduboy.readUnitName((char *)sprites[singleplayerId].name);
}

void update_settings_screen() {
    if (arduboy.justPressed(RIGHT_BUTTON)) {
        if (sprites[singleplayerId].name[usernameIdx] == '\0') {
            sprites[singleplayerId].name[usernameIdx] = ' ';
            sprites[singleplayerId].name[usernameIdx + 1] = '\0';
        }
        
        usernameIdx++;
        
        if (usernameIdx >= ARDUBOY_UNIT_NAME_LEN) {
            arduboy.writeUnitName((char *)&sprites[singleplayerId].name);
            state = TITLE;
        }
    }

    if (arduboy.justPressed(LEFT_BUTTON) && usernameIdx != 0)
        usernameIdx--;

    if (arduboy.justPressed(UP_BUTTON)) {        
        switch (sprites[singleplayerId].name[usernameIdx]) {
        case '\0':
            sprites[singleplayerId].name[usernameIdx + 1] = '\0';
        case ' ':
            sprites[singleplayerId].name[usernameIdx] = 'A';
            break;
        case 'Z':
            sprites[singleplayerId].name[usernameIdx] = 'a';
            break;
        case 'z':
            sprites[singleplayerId].name[usernameIdx] = '0';
            break;
        case '9':
            sprites[singleplayerId].name[usernameIdx] = ' ';
            break;
        default:
            sprites[singleplayerId].name[usernameIdx]++;
            break;
        }
    }

    if (arduboy.justPressed(DOWN_BUTTON)) {
        switch (sprites[id].name[usernameIdx]) {
        case '\0':
            sprites[singleplayerId].name[usernameIdx + 1] = '\0';
        case ' ':
            sprites[singleplayerId].name[usernameIdx] = '9';
            break;
        case 'A':
            sprites[singleplayerId].name[usernameIdx] = ' ';
            break;
        case 'a':
            sprites[singleplayerId].name[usernameIdx] = 'Z';
            break;
        case '0':
            sprites[singleplayerId].name[usernameIdx] = 'z';
            break;
        default:
            sprites[singleplayerId].name[usernameIdx]--;
            break;
        }
    }

    if (arduboy.justPressed(A_BUTTON | B_BUTTON)) {
        arduboy.writeUnitName((char *)&sprites[singleplayerId].name);
        state = TITLE;
    }
}

void draw_settings_screen() {
    font3x5.setCursor(CENTER_STR(sprites[id].name), 28);
 
    font3x5.print((char *)sprites[id].name);
    arduboy.drawFastHLine(CENTER_STR(sprites[id].name) + (FONT3X5_WIDTH + 1) * usernameIdx, 28 + FONT3X5_HEIGHT, FONT3X5_WIDTH);
}

void init_credits_screen() {
    creditsY = HEIGHT;
}

void update_credits_screen() {
    if ((uint8_t)arduboy.frameCount & 1) {
        creditsY--;
        if (creditsY <= -127)
            state = TITLE;
    }
    if (arduboy.justPressed(A_BUTTON | B_BUTTON))
        state = TITLE;
}

void draw_credits_screen() {
    font3x5.setCursor(20, creditsY);
    font3x5.print(F("Programming\nSublinear\nZed\n\n\n\nArt\nkenny.nl\nGriffpatch\nAmazingmax Fonts\nFilmote\n\n\nThanks to\nHaloopdy\nLode Vandevenne\nBrow1067\nBateske"));
}

bool check_game_over() {
    bool gameOver = false;
    if (millis() - gameStart > gameMillisMax) {
        gameOver = true;
    }
    for (uint8_t i = 0; i < I2C_MAX_PLAYERS; i++) {
        if (sprites[i].timeout && sprites[i].gameOver) {
            gameOver = true;
            break;
        }
    }
    sprites[id].gameOver = gameOver;
    return gameOver;
}

void update_game_over() {
    if (arduboy.justPressed(A_BUTTON | B_BUTTON)) {
        stop_multiplayer();
        state = TITLE;
    }
}

void draw_game_over() {
    int8_t eliminations[I2C_MAX_PLAYERS];
    uint8_t orderIdxs[I2C_MAX_PLAYERS];
    for (uint8_t i = 0; i < I2C_MAX_PLAYERS; i++) {
        if (sprites[i].timeout || i == id) {
            eliminations[i] = sprites[i].eliminations;
            orderIdxs[i] = i;
        } else
            eliminations[i] = -1;
    }

    for (uint8_t i = 0; i < I2C_MAX_PLAYERS - 1; i++) {
        for (uint8_t j = 0; j < I2C_MAX_PLAYERS - i - 1; j++) {
            if (eliminations[j] < eliminations[j + 1]) {
                swap(eliminations, j, j + 1);
                swap(orderIdxs, j, j + 1);
            }
        }
    }
    uint8_t playerOrder;
    for (uint8_t i = 0; i < I2C_MAX_PLAYERS; i++) {
        if (orderIdxs[i] == id) {
            playerOrder = i;
            break;
        }
    }
    font3x5.setCursor(CENTER_STR("xxx place"), 10);
    font3x5.print(playerOrder + 1);
    font3x5.print((const __FlashStringHelper *)numberSuffixes[playerOrder]);
    font3x5.print(F(" place\n"));
    font3x5.setCursor(CENTER_WIDTH(0) + STR_WIDTH(3), 23);
    font3x5.drawChar('E');
    font3x5.setX(CENTER_WIDTH(0) + STR_WIDTH(7));
    font3x5.drawChar('D');
    font3x5.setCursor(CENTER_WIDTH(STR_WIDTH(sizeof(sprites[0].name) + 6)), 30);
    for (uint8_t i = 0; i < I2C_MAX_PLAYERS; i++) {
        if (sprites[i].timeout || i == id) {
            uint8_t orderIdx = orderIdxs[i];
            font3x5.print((char *)sprites[orderIdx].name);
            font3x5.setX(CENTER_WIDTH(0) + STR_WIDTH(3));
            font3x5.print((uint8_t)sprites[orderIdx].eliminations);
            font3x5.setX(CENTER_WIDTH(0) + STR_WIDTH(7));
            font3x5.print(sprites[orderIdx].deaths);
            font3x5.newline();
        }
    }
}