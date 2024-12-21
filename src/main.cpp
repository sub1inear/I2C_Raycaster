#include "globals.h"
#include "utils.h"

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
    arduboy.waitNoButtons();
    arduboy.readUnitName((char *)sprites[nameTempBuffer].name);
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
    
    bool hit = false;
    uint8_t numPlayers = run_timeout();

    switch (state) {
    case SINGLEPLAYER_INIT:
        reset_player();
    case GAME:
        move_player();
        if (singleplayer) {
            
        } else {
            hit = handle_player_hit();
            update_multiplayer();
        }

        render();
        break;
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
        state = LOBBY;
        setup_lobby();
        break;
    case LOBBY:
        run_lobby(numPlayers);
        break;
    }

    display_fill_screen(hit ? 0xff : 0x00);
}
