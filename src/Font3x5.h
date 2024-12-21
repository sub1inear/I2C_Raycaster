#pragma once

#include <stdint.h>

#define USE_LOWER_CASE

constexpr uint8_t FONT3X5_WIDTH = 3;
constexpr uint8_t FONT3X5_HEIGHT = 6;
constexpr uint8_t FONT3X5_TRUE_HEIGHT = 8;

constexpr uint8_t FONT3X5_CHAR_BYTES = (FONT3X5_WIDTH * (FONT3X5_TRUE_HEIGHT / 8));

constexpr uint8_t CHAR_EXCLAMATION = 33;
constexpr uint8_t CHAR_PERIOD = 46;
constexpr uint8_t CHAR_MINUS = 45;
constexpr uint8_t CHAR_LETTER_A = 65;
constexpr uint8_t CHAR_LETTER_Z = 90;
constexpr uint8_t CHAR_LETTER_A_LOWER = 97;
constexpr uint8_t CHAR_LETTER_Z_LOWER = 122;
constexpr uint8_t CHAR_NUMBER_0 = 48;
constexpr uint8_t CHAR_NUMBER_9 = 57;
constexpr uint8_t CHAR_NEWLINE = 10;
constexpr uint8_t CHAR_SPACE = 32;

#ifdef USE_LOWER_CASE
  constexpr uint8_t FONT_EXCLAMATION_INDEX = 62;
  constexpr uint8_t FONT_PERIOD_INDEX = 63;
  constexpr uint8_t FONT_MINUS_INDEX = 64;
  constexpr uint8_t FONT_NUMBER_INDEX = 52;
#else
  constexpr uint8_t FONT_EXCLAMATION_INDEX = 36;
  constexpr uint8_t FONT_PERIOD_INDEX = 37;
  constexpr uint8_t FONT_MINUS_INDEX = 38;
  constexpr uint8_t FONT_NUMBER_INDEX = 26;
#endif

class Font3x5 {
  public:
    void setCursor(const int8_t x, const int8_t y) { _cursorX = _baseX = x; _cursorY = y; };
    void print(uint16_t n);
    void print(int16_t n);
    void print(const char s[]);
    void print(const __FlashStringHelper s[]);
    void drawChar(char c);
    
    void setTextColor(const uint8_t color) { _textColor = color; };
    void setHeight(const uint8_t height)   { _lineHeight = height; };

  private:
    int8_t _cursorX = 0;    // Default is 0.
    int8_t _baseX   = 0;      // needed for linebreak.
    int8_t _cursorY = 0;    // Default is 0.

    int8_t _textColor = WHITE;  // BLACK == 0, everything else is WHITE. Default is WHITE.

    uint8_t _letterSpacing = 1;  // letterSpacing controls the distance between letters. Default is 1.
    uint8_t _lineHeight    = 6;     // lineHeight controls the height between lines breakend by \n. Default is 6.

};