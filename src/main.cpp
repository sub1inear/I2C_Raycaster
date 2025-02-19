#include "globals.h"

#ifdef FPS_DEBUG
class Arduboy2Ex {
public:
    static bool nextFrameMiniDEV() {
        bool ret = arduboy.nextFrame();
        if (ret) {
            if (arduboy.lastFrameDurationMs > arduboy.eachFrameMillis) {
                arduboy.digitalWriteRGB(RGB_ON, RGB_OFF, RGB_OFF);
            } else {
                arduboy.digitalWriteRGB(RGB_OFF, RGB_ON, RGB_OFF);
            }
        }
        return ret;
    }
};
#endif

void setup() {
    arduboy.boot();
    arduboy.safeMode();
    arduboy.waitNoButtons();

    sprites[0].id = 0;
    sprites[0].eliminations = 0;
    sprites[0].timeout = 1;
    strcpy(sprites[0].name, "a");

    sprites[1].id = 0;
    sprites[1].eliminations = 2;
    sprites[1].timeout = 1;
    strcpy(sprites[1].name, "b");

    sprites[2].id = 0;
    sprites[2].eliminations = 1;
    sprites[2].timeout = 1;
    strcpy(sprites[2].name, "c");

    sprites[3].id = 0;
    sprites[3].eliminations = 0;
    sprites[3].timeout = 1;
    strcpy(sprites[3].name, "d");

    draw_game_over();
    display_fill_screen(false, 0x00);
    while (true) {}
}

void loop() {
#ifdef FPS_DEBUG
    if (!Arduboy2Ex::nextFrameMiniDEV()) {
        return;
    }
#else
    if (!arduboy.nextFrameDEV()) {
        return;
    }
#endif

    arduboy.pollButtons();
    
    bool flash = false;
    
    switch (state) {
    case TITLE:
        update_title_screen();
        draw_title_screen();
        break;
    case CREDITS_INIT:
        state = CREDITS;
        init_credits_screen();
    case CREDITS:
        update_credits_screen();
        draw_credits_screen();
        break;
    case SETTINGS_INIT:
        init_settings_screen();
        state = SETTINGS;
    case SETTINGS:
        update_settings_screen();
        draw_settings_screen();
        break;
    case LOBBY_INIT:
        setup_lobby();
        break;
    case LOBBY: {
        uint8_t numPlayers = run_timeout();
        run_lobby(numPlayers);
        break;
    }
    case GAME_INIT:
        wipe_effect();
        init_fast_random_seed();
        init_powerups();
        if (singleplayer) {
            id = singleplayerId;
            init_ais();
        } else
            start_multiplayer();
        init_player();
        gameTimer = 0;
        state = GAME;
    case GAME:
        update_player();
        flash |= update_powerups();
        if (singleplayer)
            update_ais();
        else {
            update_multiplayer();
            run_timeout();
        }
        render();
        flash |= receive_multiplayer();
        if (check_game_over())
            state = GAME_OVER_INIT;
        break;
    case GAME_OVER_INIT:
        wipe_effect();
        state = GAME_OVER;
    case GAME_OVER:
        update_game_over();
        draw_game_over();
        break;
    }
    bool clear = state != GAME_INIT && state != GAME_OVER_INIT;
    display_fill_screen(clear, flash ? 0xff : 0x00);
}
