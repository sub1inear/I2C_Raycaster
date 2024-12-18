#pragma once
#include "utils.h"

constexpr uint8_t laserTaggerWidth = 50;
constexpr uint8_t laserTaggerHeight = 32;

constexpr int8_t laserTaggerOffsetX = 12;

constexpr uint8_t laserTaggerX = (WIDTH - laserTaggerWidth) / 2 + laserTaggerOffsetX;
constexpr uint8_t laserTaggerY = HEIGHT - laserTaggerHeight;

const unsigned char PROGMEM laserTaggerPlusMask[] = {
    laserTaggerWidth, laserTaggerHeight,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0x60, 0xe0, 0xa0, 0xf0, 0xd8, 0xf8, 0xa8, 0xfc, 0x74, 0xfc, 0xa8, 0xfc, 0xd0, 0xfc, 0xa0, 0xfc, 0x40, 0xfc, 0x80, 0xfc, 0x00, 0xfc, 0x00, 0xfc, 0x0a, 0xff, 0x1f, 0xff, 
    0x2a, 0xff, 0x7f, 0xff, 0xaa, 0xff, 0xff, 0xff, 0xaa, 0xff, 0x5f, 0xff, 0x4a, 0xfe, 0x46, 0xfe, 0xbc, 0xfc, 0x44, 0xfc, 0x48, 0xfc, 0x50, 0xf8, 0xa0, 0xf0, 0x00, 0xe0, 0x00, 0xc0, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xaa, 0xff, 0x55, 0xff, 0x2e, 0xff, 0x55, 0xff, 0x2a, 0xff, 0x55, 0xff, 0xaa, 0xff, 0x55, 0xff, 0xaa, 0xff, 0x57, 0xff, 0xaa, 0xff, 0x55, 0xff, 0xaa, 0xff, 0x54, 0xff, 0xa8, 0xff, 
    0x50, 0xff, 0xa0, 0xff, 0x40, 0xff, 0x01, 0xff, 0x42, 0xff, 0x07, 0xff, 0x42, 0xff, 0x04, 0xff, 0x43, 0xff, 0x04, 0xff, 0x42, 0xff, 0x05, 0xff, 0x43, 0xff, 0x00, 0xff, 0x40, 0xff, 0x00, 0xff, 0x40, 0xff, 0x00, 0xfe, 0x40, 0xfc, 0x00, 0xf8, 0x00, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0xe0, 0xa0, 0xf0, 0xd8, 0xf8, 0xa8, 0xfc, 0xfe, 0xfe, 0xaa, 0xff, 0xfd, 0xff, 0xba, 0xff, 0xf4, 0xff, 0x80, 0xff, 0x50, 0xff, 0x00, 0xff, 0x01, 0xff, 0x08, 0xff, 0x55, 0xff, 0xaa, 0xff, 0x55, 0xff, 0xaa, 0xff, 0x55, 0xff, 0xaa, 0xff, 0x51, 0xff, 0xa2, 0xff, 
    0x45, 0xff, 0x8a, 0xff, 0x55, 0xff, 0xaa, 0xff, 0x54, 0xff, 0xa8, 0xff, 0x54, 0xff, 0xe8, 0xff, 0x54, 0xff, 0x08, 0xff, 0x14, 0xff, 0x88, 0xff, 0x14, 0xff, 0x88, 0xff, 0x14, 0xff, 0x80, 0xff, 0x14, 0xff, 0x80, 0xff, 0x14, 0xff, 0x80, 0xff, 0x54, 0xff, 0xa8, 0xff, 0x54, 0xfe, 0xa8, 0xfc, 0x00, 0x00, 
    0x80, 0xc0, 0x54, 0xfc, 0xaa, 0xfe, 0x55, 0xff, 0xaa, 0xff, 0x5d, 0xff, 0xaa, 0xff, 0x7f, 0xff, 0xea, 0xff, 0xdd, 0xff, 0xaa, 0xff, 0xff, 0xff, 0xaa, 0xff, 0xdd, 0xff, 0xaa, 0xff, 0xf4, 0xff, 0xa8, 0xff, 0xd0, 0xff, 0xa0, 0xff, 0x81, 0xff, 0x02, 0xff, 0x05, 0xff, 0x0a, 0xff, 0x15, 0xff, 0x2a, 0xff, 
    0x55, 0xff, 0xaa, 0xff, 0x55, 0xff, 0xaa, 0xff, 0x55, 0xff, 0xaa, 0xff, 0x55, 0xff, 0xee, 0xff, 0x55, 0xff, 0x00, 0xff, 0x00, 0xff, 0xaa, 0xff, 0x55, 0xff, 0x00, 0xff, 0x01, 0xff, 0x00, 0xff, 0x01, 0xff, 0x00, 0xff, 0x55, 0xff, 0xaa, 0xff, 0x00, 0xff, 0x2a, 0xff, 0x55, 0xff, 0xaa, 0xff, 0x40, 0xe0,
};


constexpr uint8_t laserTaggerFlashWidth = 29;
constexpr uint8_t laserTaggerFlashHeight = 29;

constexpr int8_t laserTaggerFlashOffsetX = 0;
constexpr int8_t laserTaggerFlashOffsetY = -5;

const unsigned char PROGMEM laserTaggerFlashPlusMask[] = {
    laserTaggerFlashWidth, laserTaggerFlashHeight,
    0x00, 0x00, 0x00, 0xc0, 0x00, 0xe0, 0x40, 0xe0, 0x80, 0xf0, 0xd0, 0xf8, 0xa0, 0xf8, 0xf4, 0xfc, 0xe8, 0xfe, 0xfc, 0xff, 0xf8, 0xff, 0xfd, 0xff, 0xea, 0xff, 0xfc, 0xff, 0xfa, 
    0xff, 0xff, 0xff, 0xee, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xee, 0xff, 0xfd, 0xff, 0xfa, 0xff, 0xfd, 0xff, 0xf8, 0xff, 0xfc, 0xff, 0xf8, 0xfe, 0xf4, 0xfe, 0xe8, 0xfe, 
    0x80, 0xfc, 0xd4, 0xff, 0xfa, 0xff, 0xff, 0xff, 0xee, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0x0a, 0xff, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0x00, 0x07, 0x05, 0x1f, 0x03, 0x1f, 0x1f, 0x1f, 0x0f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x0f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 
    0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x0f, 0x1f,
};

const uint8_t texSize = 16;    // should be even

const uint8_t _texData[texSize * texSize/8] {
    0x00, 0x00, 0x30, 0xe8, 0x80, 0x3c, 0x4e, 0x4e, 0x4e, 0x4e, 0x3c, 0x80, 0xe8, 0x30, 0x00, 0x00, 
    0x00, 0x00, 0x1f, 0x19, 0x01, 0x73, 0x17, 0x17, 0x17, 0x17, 0x73, 0x01, 0x19, 0x1f, 0x00, 0x00,
};

const uint8_t _texMask[texSize * texSize/8] {
    0x00, 0x00, 0x30, 0xf8, 0xf8, 0xfc, 0xfe, 0xfe, 0xfe, 0xfe, 0xfc, 0xf8, 0xf8, 0x30, 0x00, 0x00, 
    0x00, 0x00, 0x1f, 0x19, 0x01, 0x7b, 0x1f, 0x1f, 0x1f, 0x1f, 0x7b, 0x01, 0x19, 0x1f, 0x00, 0x00,
};
