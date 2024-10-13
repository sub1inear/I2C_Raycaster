#pragma once

#include <stdint.h>

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
    uint8_t _lineHeight    = 8;     // lineHeight controls the height between lines breakend by \n. Default is 8.

};