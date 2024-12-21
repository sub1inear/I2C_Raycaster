#include "multiplayer.h"
#define I2C_IMPLEMENTATION
#include "globals.h"
#include "utils.h"

void onReceive() {
    uint8_t *buffer = I2C::getBuffer();
    uint8_t id = ((sprite_t *)buffer)->id;
    uint8_t *sprite = (uint8_t *)&sprites[id];
    for (uint8_t i = 0; i < sizeof(sprite_t) - 1; i++) { // copy all but id
        sprite[i] = buffer[i];
    }
    sprites[id].timeout = 1;
}
void handshakeOnRequest() {
    I2C::transmit(&state);
}
void handshakeOnReceive() {
    uint8_t *buffer = I2C::getBuffer();
    if (buffer[0] == nullId) {
        state = GAME;
        I2C::onReceive(onReceive);
    } else {
        sprites[buffer[0]].timeout = 1;
    }
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
        I2C::read(I2C::getAddressFromId(id), &state); // copy state from other player
        switch (I2C::getTWError()) {
        case TW_MR_SLA_NACK:
            I2C::setAddress(I2C::getAddressFromId(id), true);

            sprites[id].id = id;
            arduboy.readUnitName((char *)sprites[id].name);
            I2C::onReceive(handshakeOnReceive);
            I2C::onRequest(handshakeOnRequest);
            reset_player();
            return;
        case TW_SUCCESS:
            id--;
            break;
        }
    }
    state = TITLE;
    return;
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
    font3x5.print(F(" of " STR(I2C_MAX_PLAYERS) " players have joined\nPress A to start\n"));
    font3x5.print(id);
    if (arduboy.pressed(A_BUTTON)) {
        uint8_t message = nullId;
        I2C::write(0x00, &message, true);
        state = GAME;
        I2C::onReceive(onReceive);
    }
}
void update_multiplayer() {
    // send data over I2C
    I2C::write(0x00, &sprites[id], false);
}