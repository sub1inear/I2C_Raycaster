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
}

void setup_multiplayer() {
    I2C::init();

    id = I2C::handshake();
    if (id == I2C_HANDSHAKE_FAILED) {
        arduboy.exitToBootloader();
    }
    sprites[id].id = id;
    arduboy.readUnitName((char *)sprites[id].name);
    I2C::onReceive(onReceive);
    
    // lobby code wip
    // uint8_t message[] = {false, 0xFF};
    // for (uint8_t i = 0; i < I2C_MAX_PLAYERS - 1; ) {
    //     I2C::write(I2C::getAddressFromId(i), message, 2, true)
    //     switch (I2C::getTWError()) {
    //     case TW_MT_SLA_NACK:
    //         I2C::setAddress(I2C::getAddressFromId(i));
    //         I2C::onReceive(handshakeOnReceive);
    //         break;
    //     case TW_SUCCESS:
    //         i++;
    //     }
    // }
}

void update_multiplayer() {
    // send data over I2C
    I2C::write(0x0, &sprites[id], false);
}