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
}

void loop() {
#ifdef FPS_DEBUG
    if (!Arduboy2Ex::nextFrameMiniDEV()) {
        return;
    }
#else
    if (!arduboy.nextFrame()) {
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
        wipe_effect();
        setup_lobby();
        break;
    case LOBBY: {
        uint8_t numPlayers = run_timeout();
        run_lobby(numPlayers);
        break;
    }
    case GAME_INIT:
        init_fast_random_seed();
        init_powerups();
        if (singleplayer) {
            wipe_effect();
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
    // cannot clear tick before wipe_effect
    bool clear = true;
    switch (state) {
    case GAME_INIT:
        if (singleplayer)
            clear = false;
        break;
    case GAME_OVER_INIT:
    case LOBBY_INIT:
        clear = false;
        break;
    default:
        break;
    }
    display_fill_screen(clear, flash ? 0xff : 0x00);
}
