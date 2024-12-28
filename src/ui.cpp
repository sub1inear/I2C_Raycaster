#include "ui.h"
#include "globals.h"
#include "utils.h"

const State onClick[4] PROGMEM = {GAME_INIT, LOBBY_INIT, SETTINGS_INIT, CREDITS_INIT};

void draw_health_bar() {
}

void update_title_screen() {
    menuIdx += arduboy.justPressed(DOWN_BUTTON) - arduboy.justPressed(UP_BUTTON);
    menuIdx = tclamp(menuIdx, (int8_t)0, (int8_t)3);
    if (arduboy.justReleased(A_BUTTON)) {
        state = pgm_read_byte(&onClick[menuIdx]);
        singleplayer = state == GAME_INIT;
    }
}

void draw_title_screen() {
    arduboy.drawBitmap(CENTER_WIDTH(titleImageWidth), 0, titleImage, titleImageWidth, titleImageHeight);
    font3x5.setCursor(CENTER_STR("Singleplayer"), 41);
    font3x5.print(F("Singleplayer\nMultiplayer\nSettings\nCredits"));
    arduboy.drawBitmap(35, 41 + menuIdx * FONT3X5_HEIGHT, menuImage, menuImageWidth, menuImageHeight);
}

void init_settings_screen() {
    usernameIdx = 0;
}

void update_settings_screen() {

    if (arduboy.justPressed(RIGHT_BUTTON)) {
        if (sprites[nameTempBuffer].name[usernameIdx] == '\0')
            sprites[nameTempBuffer].name[usernameIdx] = ' ';
        
        usernameIdx++;
        
        if (usernameIdx > ARDUBOY_UNIT_NAME_LEN) {
            arduboy.writeUnitName((char *)&sprites[nameTempBuffer].name);
            state = TITLE;
        }
    }

    if (arduboy.justPressed(LEFT_BUTTON) && usernameIdx != 0) {
        usernameIdx--;
    }

    if (arduboy.justPressed(UP_BUTTON)) {        
        switch (sprites[nameTempBuffer].name[usernameIdx]) {
        case '\0':
        case ' ':
            sprites[nameTempBuffer].name[usernameIdx] = 'A';
            break;
        case 'Z':
            sprites[nameTempBuffer].name[usernameIdx] = 'a';
            break;
        case 'z':
            sprites[nameTempBuffer].name[usernameIdx] = '0';
            break;
        case '9':
            sprites[nameTempBuffer].name[usernameIdx] = ' ';
            break;
        default:
            sprites[nameTempBuffer].name[usernameIdx]++;
            break;
        }
    }

    if (arduboy.justPressed(DOWN_BUTTON)) {
        switch (sprites[id].name[usernameIdx]) {
        case '\0':
        case ' ':
            sprites[nameTempBuffer].name[usernameIdx] = '9';
            break;
        case 'A':
            sprites[nameTempBuffer].name[usernameIdx] = ' ';
            break;
        case 'a':
            sprites[nameTempBuffer].name[usernameIdx] = 'Z';
            break;
        case '0':
            sprites[nameTempBuffer].name[usernameIdx] = 'z';
            break;
        default:
            sprites[nameTempBuffer].name[usernameIdx]--;
            break;
        }
    }

    if (arduboy.pressed(B_BUTTON)) {
        arduboy.writeUnitName((char *)&sprites[nameTempBuffer].name);
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
}

void draw_credits_screen() {
    font3x5.setCursor(0, creditsY);
    font3x5.print(F("Programming\nSublinear\nZed\n\n\n\n\n\n\n\n\n\n\nArt\nkenny.nl\ngriffpatch"));
}