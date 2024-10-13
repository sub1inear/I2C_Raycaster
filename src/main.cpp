#include "globals.h"
#include "utils.h"

#define FPS_DEBUG

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

    setup_multiplayer();

    reset_player();

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

    move_player();
    bool hit = handle_player_hit();

    update_multiplayer();

    render();

    display_fill_screen(hit ? 0xff : 0x00);
}
