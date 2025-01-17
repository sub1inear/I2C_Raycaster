#define I2C_IMPLEMENTATION
#include "globals.h"

void on_receive() {
    uint8_t *buffer = I2C::getBuffer();
    uint8_t id = ((sprite_t *)buffer)->id;
    uint8_t *sprite = (uint8_t *)&sprites[id];
    for (uint8_t i = 0; i < sizeof(sprite_t) - 1; i++) { // copy all but id
        sprite[i] = buffer[i];
    }
    sprites[id].timeout = 1;
}
void handshake_on_request() {
    I2C::transmit(&state);
}
void handshake_on_receive() {
    uint8_t *buffer = I2C::getBuffer();
    if (buffer[0] == nullId) {
        state = GAME_INIT;
    } else {
        sprites[buffer[0]].timeout = 1;
    }
}

void start_multiplayer() {
    I2C::onReceive(on_receive);
}

void dummy_callback() {
}

void stop_multiplayer() {
    I2C::onReceive(dummy_callback);
    I2C::onRequest(dummy_callback);
    for (uint8_t i = 0; i < I2C_MAX_PLAYERS; i++)
        sprites[i].timeout = 0;
}

void setup_lobby() {
    I2C::init();

    if (I2C::detectEmulator()) {
        font3x5.setCursor(CENTER_STR("Emulator does not support"), 20);
        font3x5.print(F("Emulator does not support\n    I2C multiplayer."));
        display_fill_screen(0x00);
        while (arduboy.notPressed(B_BUTTON)) { }
        state = TITLE;
        return;
    }

    // handshake
    for (id = I2C_MAX_PLAYERS - 1; id >= 0; ) {
        uint8_t otherPlayerState;
        I2C::read(I2C::getAddressFromId(id), &otherPlayerState);
        switch (I2C::getTWError()) {
        case TW_MR_SLA_NACK:
            I2C::setAddress(I2C::getAddressFromId(id), true);

            sprites[id].id = id;
            arduboy.readUnitName((char *)sprites[id].name);
            I2C::onReceive(handshake_on_receive);
            I2C::onRequest(handshake_on_request);
            state = LOBBY;
            return;
        case TW_SUCCESS:
            id--;
            switch (otherPlayerState) {
            case GAME_INIT:
            case GAME:
                state = GAME_INIT;
                break;
            }
            break;
        }
    }
    state = TITLE;
}

uint8_t run_timeout() {
    uint8_t numPlayers = 1;
    for (uint8_t i = 0; i < I2C_MAX_PLAYERS; i++) {
        if (sprites[i].timeout) {
            sprites[i].timeout++;
            numPlayers++;
        }
    }
    return numPlayers;
}

void run_lobby(uint8_t numPlayers) {
    I2C::write(0x00, &id, false);
        
    font3x5.setCursor(0, 0);
    font3x5.print(numPlayers);
    font3x5.print(F(" of " STR(I2C_MAX_PLAYERS) " players have joined\nPress A to start\nId "));
    font3x5.print(id);
    if (arduboy.justPressed(A_BUTTON)) {
        uint8_t message = nullId;
        I2C::write(0x00, &message, true);
        state = GAME_INIT;
    }
    if (arduboy.pressed(B_BUTTON)) {
        stop_multiplayer();
        state = TITLE;
    }
}
void update_multiplayer() {
    // send data over I2C
    I2C::write(0x00, &sprites[id], false);
}