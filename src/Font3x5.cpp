#include <Arduboy2.h>
#include "Font3x5.h"

extern Arduboy2 arduboy;

const uint8_t PROGMEM font_images[] = {  
  // #65 Letter 'A'.
  0x1F,  // ░░░▓▓▓▓▓
  0x05,  // ░░░░░▓░▓
  0x1F,  // ░░░▓▓▓▓▓

  // #66 Letter 'B'.
  0x1F,  // ░░░▓▓▓▓▓
  0x15,  // ░░░▓░▓░▓
  0x1B,  // ░░░▓▓░▓▓

  // #67 Letter 'C'.
  0x1F,  // ░░░▓▓▓▓▓
  0x11,  // ░░░▓░░░▓
  0x11,  // ░░░▓░░░▓

  // #68 Letter 'D'.
  0x1F,  // ░░░▓▓▓▓▓
  0x11,  // ░░░▓░░░▓
  0x0E,  // ░░░░▓▓▓░

  // #69 Letter 'E'.
  0x1F,  // ░░░▓▓▓▓▓
  0x15,  // ░░░▓░▓░▓
  0x11,  // ░░░▓░░░▓

  // #70 Letter 'F'.
  0x1F,  // ░░░▓▓▓▓▓
  0x05,  // ░░░░░▓░▓
  0x01,  // ░░░░░░░▓

  // #71 Letter 'G'.
  0x1F,  // ░░░▓▓▓▓▓
  0x11,  // ░░░▓░░░▓
  0x1D,  // ░░░▓▓▓░▓

  // #72 Letter 'H'.
  0x1F,  // ░░░▓▓▓▓▓
  0x04,  // ░░░░░▓░░
  0x1F,  // ░░░▓▓▓▓▓

  // #73 Letter 'I'.
  0x00,  // ░░░░░░░░
  0x1F,  // ░░░▓▓▓▓▓
  0x00,  // ░░░░░░░░

  // #74 Letter 'J'.
  0x10,  // ░░░▓░░░░
  0x10,  // ░░░▓░░░░
  0x1F,  // ░░░▓▓▓▓▓

  // #75 Letter 'K'.
  0x1F,  // ░░░▓▓▓▓▓
  0x04,  // ░░░░░▓░░
  0x1B,  // ░░░▓▓░▓▓

  // #76 Letter 'L'.
  0x1F,  // ░░░▓▓▓▓▓
  0x10,  // ░░░▓░░░░
  0x10,  // ░░░▓░░░░

  // #77 Letter 'M'.
  0x1F,  // ░░░▓▓▓▓▓
  0x06,  // ░░░░░▓▓░
  0x1F,  // ░░░▓▓▓▓▓

  // #78 Letter 'N'.
  0x1F,  // ░░░▓▓▓▓▓
  0x01,  // ░░░░░░░▓
  0x1F,  // ░░░▓▓▓▓▓

  // #79 Letter 'O'.
  0x1F,  // ░░░▓▓▓▓▓
  0x11,  // ░░░▓░░░▓
  0x1F,  // ░░░▓▓▓▓▓

  // #80 Letter 'P'.
  0x1F,  // ░░░▓▓▓▓▓
  0x05,  // ░░░░░▓░▓
  0x07,  // ░░░░░▓▓▓

  // #81 Letter 'Q'.
  0x1F,  // ░░░▓▓▓▓▓
  0x31,  // ░░▓▓░░░▓
  0x1F,  // ░░░▓▓▓▓▓

  // #82 Letter 'R'.
  0x1F,  // ░░░▓▓▓▓▓
  0x05,  // ░░░░░▓░▓
  0x1B,  // ░░░▓▓░▓▓

  // #83 Letter 'S'.
  0x17,  // ░░░▓░▓▓▓
  0x15,  // ░░░▓░▓░▓
  0x1D,  // ░░░▓▓▓░▓

  // #84 Letter 'T'.
  0x01,  // ░░░░░░░▓
  0x1F,  // ░░░▓▓▓▓▓
  0x01,  // ░░░░░░░▓

  // #85 Letter 'U'.
  0x1F,  // ░░░▓▓▓▓▓
  0x10,  // ░░░▓░░░░
  0x1F,  // ░░░▓▓▓▓▓

  // #86 Letter 'V'.
  0x0F,  // ░░░░▓▓▓▓
  0x10,  // ░░░▓░░░░
  0x0F,  // ░░░░▓▓▓▓

  // #87 Letter 'W'.
  0x1F,  // ░░░▓▓▓▓▓
  0x0C,  // ░░░░▓▓░░
  0x1F,  // ░░░▓▓▓▓▓

  // #88 Letter 'X'.
  0x1B,  // ░░░▓▓░▓▓
  0x04,  // ░░░░░▓░░
  0x1B,  // ░░░▓▓░▓▓

  // #89 Letter 'Y'.
  0x07,  // ░░░░░▓▓▓
  0x1C,  // ░░░▓▓▓░░
  0x07,  // ░░░░░▓▓▓

  // #90 Letter 'Z'.
  0x19,  // ░░░▓▓░░▓
  0x15,  // ░░░▓░▓░▓
  0x13,  // ░░░▓░░▓▓

  #ifdef USE_LOWER_CASE
  
    // #97 Letter 'a'.
    0x0C,  // ░░░░▓▓░░
    0x12,  // ░░░▓░░▓░
    0x1E,  // ░░░▓▓▓▓░

    // #98 Letter 'b'.
    0x1F,  // ░░░▓▓▓▓▓
    0x12,  // ░░░▓░░▓░
    0x0C,  // ░░░░▓▓░░

    // #99 Letter 'c'.
    0x1E,  // ░░░▓▓▓▓░
    0x12,  // ░░░▓░░▓░
    0x12,  // ░░░▓░░▓░

    // #100 Letter 'd'.
    0x0C,  // ░░░░▓▓░░
    0x12,  // ░░░▓░░▓░
    0x1F,  // ░░░▓▓▓▓▓

    // #101 Letter 'e'.
    0x0C,  // ░░░░▓▓░░
    0x1A,  // ░░░▓▓░▓░
    0x14,  // ░░░▓░▓░░

    // #102 Letter 'f'.
    0x04,  // ░░░░░▓░░
    0x1F,  // ░░░▓▓▓▓▓
    0x05,  // ░░░░░▓░▓

    // #103 Letter 'g'.
    0x2E,  // ░░▓░▓▓▓░
    0x2A,  // ░░▓░▓░▓░
    0x1E,  // ░░░▓▓▓▓░

    // #104 Letter 'h'.
    0x1F,  // ░░░▓▓▓▓▓
    0x02,  // ░░░░░░▓░
    0x1C,  // ░░░▓▓▓░░

    // #105 Letter 'i'.
    0x00,  // ░░░░░░░░
    0x1D,  // ░░░▓▓▓░▓
    0x00,  // ░░░░░░░░

    // #106 Letter 'j'.
    0x20,  // ░░▓░░░░░
    0x1D,  // ░░░▓▓▓░▓
    0x00,  // ░░░░░░░░

    // #107 Letter 'k'.
    0x1F,  // ░░░▓▓▓▓▓
    0x04,  // ░░░░░▓░░
    0x1A,  // ░░░▓▓░▓░

    // #108 Letter 'l'.
    0x01,  // ░░░░░░░▓
    0x1F,  // ░░░▓▓▓▓▓
    0x00,  // ░░░░░░░░

    // #109 Letter 'm'.
    0x1E,  // ░░░▓▓▓▓░
    0x04,  // ░░░░░▓░░
    0x1E,  // ░░░▓▓▓▓░

    // #110 Letter 'n'.
    0x1E,  // ░░░▓▓▓▓░
    0x02,  // ░░░░░░▓░
    0x1E,  // ░░░▓▓▓▓░

    // #111 Letter 'o'.
    0x1E,  // ░░░▓▓▓▓░
    0x12,  // ░░░▓░░▓░
    0x1E,  // ░░░▓▓▓▓░

    // #112 Letter 'p'.
    0x3E,  // ░░▓▓▓▓▓░
    0x12,  // ░░░▓░░▓░
    0x0C,  // ░░░░▓▓░░

    // #113 Letter 'q'.
    0x0C,  // ░░░░▓▓░░
    0x12,  // ░░░▓░░▓░
    0x3E,  // ░░▓▓▓▓▓░

    // #114 Letter 'r'.
    0x1E,  // ░░░▓▓▓▓░
    0x02,  // ░░░░░░▓░
    0x06,  // ░░░░░▓▓░

    // #115 Letter 's'.
    0x14,  // ░░░▓░▓░░
    0x12,  // ░░░▓░░▓░
    0x0A,  // ░░░░▓░▓░

    // #116 Letter 't'.
    0x02,  // ░░░░░░▓░
    0x0F,  // ░░░░▓▓▓▓
    0x12,  // ░░░▓░░▓░

    // #117 Letter 'u'.
    0x1E,  // ░░░▓▓▓▓░
    0x10,  // ░░░▓░░░░
    0x1E,  // ░░░▓▓▓▓░

    // #118 Letter 'v'.
    0x0E,  // ░░░░▓▓▓░
    0x10,  // ░░░▓░░░░
    0x0E,  // ░░░░▓▓▓░

    // #119 Letter 'w'.
    0x1E,  // ░░░▓▓▓▓░
    0x08,  // ░░░░▓░░░
    0x1E,  // ░░░▓▓▓▓░

    // #120 Letter 'x'.
    0x1A,  // ░░░▓▓░▓░
    0x04,  // ░░░░░▓░░
    0x1A,  // ░░░▓▓░▓░

    // #121 Letter 'y'.
    0x2E,  // ░░▓░▓▓▓░
    0x28,  // ░░▓░▓░░░
    0x1E,  // ░░░▓▓▓▓░

    // #122 Letter 'z'.
    0x1A,  // ░░░▓▓░▓░
    0x12,  // ░░░▓░░▓░
    0x16,  // ░░░▓░▓▓░

  #endif

  // #48 Number '0'.
  0x1F,  // ░░░▓▓▓▓▓
  0x11,  // ░░░▓░░░▓
  0x1F,  // ░░░▓▓▓▓▓

  // #49 Number '1'.
  0x00,  // ░░░░░░░░
  0x1F,  // ░░░▓▓▓▓▓
  0x00,  // ░░░░░░░░

  // #50 Number '2'.
  0x1D,  // ░░░▓▓▓░▓
  0x15,  // ░░░▓░▓░▓
  0x17,  // ░░░▓░▓▓▓

  // #51 Number '3'.
  0x11,  // ░░░▓░░░▓
  0x15,  // ░░░▓░▓░▓
  0x1F,  // ░░░▓▓▓▓▓

  // #52 Number '4'.
  0x07,  // ░░░░░▓▓▓
  0x04,  // ░░░░░▓░░
  0x1F,  // ░░░▓▓▓▓▓

  // #53 Number '5'.
  0x17,  // ░░░▓░▓▓▓
  0x15,  // ░░░▓░▓░▓
  0x1D,  // ░░░▓▓▓░▓

  // #54 Number '6'.
  0x1F,  // ░░░▓▓▓▓▓
  0x15,  // ░░░▓░▓░▓
  0x1D,  // ░░░▓▓▓░▓

  // #55 Number '7'.
  0x01,  // ░░░░░░░▓
  0x01,  // ░░░░░░░▓
  0x1F,  // ░░░▓▓▓▓▓

  // #56 Number '8'.
  0x1F,  // ░░░▓▓▓▓▓
  0x15,  // ░░░▓░▓░▓
  0x1F,  // ░░░▓▓▓▓▓

  // #57 Number '9'.
  0x17,  // ░░░▓░▓▓▓
  0x15,  // ░░░▓░▓░▓
  0x1F,  // ░░░▓▓▓▓▓

  // #33 Symbol '!'.
  0x00,  // ░░░░░░░░
  0x17,  // ░░░▓░▓▓▓
  0x00,  // ░░░░░░░░

  // #46 Symbol '.'.
  0x00,  // ░░░░░░░░
  0x10,  // ░░░▓░░░░
  0x00,  // ░░░░░░░░
  
  // #45 Symbol '-'.
  0x04,
  0x04,
  0x04, 
};

constexpr static uint8_t NDIGITS = 5;

constexpr static uint16_t digitLut[NDIGITS] PROGMEM = {
    1u,
    10u,
    100u,
    1000u,
    10000u,
};

void Font3x5::print(uint16_t n) {
    bool found = false;
    // loop through digits, highest to lowest
    for (int8_t i = NDIGITS - 1; i >= 0; i--) {
        char c = FONT_NUMBER_INDEX;
        while (n >= pgm_read_word(&digitLut[i])) {
            n -= pgm_read_word(&digitLut[i]);
            c++;
        }
        // skip leading zeros
        found |= (c != FONT_NUMBER_INDEX);
        if (found || i == 0) {
            arduboy.drawBitmap(_cursorX, _cursorY, &font_images[c * FONT3X5_CHAR_BYTES], FONT3X5_WIDTH, FONT3X5_HEIGHT, _textColor);
            _cursorX += FONT3X5_WIDTH + _letterSpacing;
        }
    }
}

void Font3x5::print(int16_t n) {
    if (n < 0) {
        arduboy.drawBitmap(_cursorX, _cursorY, &font_images[FONT_MINUS_INDEX * FONT3X5_CHAR_BYTES], FONT3X5_WIDTH, FONT3X5_HEIGHT, _textColor);
        _cursorX += FONT3X5_WIDTH + _letterSpacing;
    }
    print((uint16_t)abs(n));
}

void Font3x5::drawChar(char c) {
    int8_t imageIdx = -1;
        
    switch (c) {
    case CHAR_LETTER_A ... CHAR_LETTER_Z:
        imageIdx = c - CHAR_LETTER_A;
        break;
#ifdef USE_LOWER_CASE    
    case CHAR_LETTER_A_LOWER ... CHAR_LETTER_Z_LOWER:
        imageIdx = c - CHAR_LETTER_A_LOWER + 26;
        break;
#endif
    case CHAR_NUMBER_0 ... CHAR_NUMBER_9:
        imageIdx = c - CHAR_NUMBER_0 + FONT_NUMBER_INDEX;
        break;
    
    case CHAR_EXCLAMATION:
        imageIdx = FONT_EXCLAMATION_INDEX;
        break;
    case CHAR_PERIOD:
        imageIdx = FONT_PERIOD_INDEX;
        break;
    case CHAR_MINUS:
        imageIdx = FONT_MINUS_INDEX;
        break;
    case CHAR_NEWLINE:
        _cursorY += _lineHeight;
        _cursorX = _baseX;
        return;
    }
    if (imageIdx > -1) {
        arduboy.drawBitmap(_cursorX, _cursorY, &font_images[imageIdx * FONT3X5_CHAR_BYTES], FONT3X5_WIDTH, FONT3X5_HEIGHT, _textColor);
    }
    _cursorX += FONT3X5_WIDTH + _letterSpacing;
}

void Font3x5::print(const char s[]) {
    uint8_t stringIdx = 0;
    while (s[stringIdx] > 0) {
        char c = s[stringIdx];
        drawChar(c);
        stringIdx++;  
    }
}
void Font3x5::print(const __FlashStringHelper s[]) {
    uint8_t stringIdx = 0;
    const char *progString = reinterpret_cast<const char *>(s);
    while (pgm_read_byte(&progString[stringIdx]) > 0) {
        char c = pgm_read_byte(&progString[stringIdx]);
        drawChar(c);
        stringIdx++;
    }
}