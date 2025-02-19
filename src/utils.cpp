#include "globals.h"

void display_fill_screen(uint8_t color) {
    // modified from Arduboy2 lib
    uint16_t count;
    
    uint8_t *ptr = Arduboy2::sBuffer;

    asm volatile (
        "   ldi   %A[count], %[len_lsb]               \n\t" //for (len = WIDTH * HEIGHT / 8)
        "   ldi   %B[count], %[len_msb]               \n\t"
        "1: ld    __tmp_reg__, %a[ptr]      ;2        \n\t" //tmp = *(image)
        "   out   %[spdr], __tmp_reg__      ;1        \n\t" //SPDR = tmp
        "   rjmp .+0                        ;2        \n\t"
        "2: sbiw  %A[count], 1              ;2        \n\t" //len --
        "   sbrc  %A[count], 0              ;1/2      \n\t" //loop twice for cheap delay
        "   rjmp  2b                        ;2        \n\t"
        "   st    %a[ptr]+, %[color]        ;2        \n\t" //*(image++) = color
        "   brne  1b                        ;1/2 :18  \n\t" //len > 0
        "   in    __tmp_reg__, %[spsr]                \n\t" //read SPSR to clear SPIF
        : [ptr]     "+&e" (ptr),
          [count]   "=&w" (count)
        : [spdr]    "I"   (_SFR_IO_ADDR(SPDR)),
          [spsr]    "I"   (_SFR_IO_ADDR(SPSR)),
          [len_msb] "M"   (WIDTH * (HEIGHT / 8 * 2) >> 8),   // 8: pixels per byte
          [len_lsb] "M"   (WIDTH * (HEIGHT / 8 * 2) & 0xFF), // 2: for delay loop multiplier
          [color]   "r"   (color)
    );
}

void init_fast_random_seed() {
    // modified from Arduboy2 lib
    power_adc_enable(); // ADC on

    // do an ADC read from an unconnected input pin
    ADCSRA |= _BV(ADSC); // start conversion (ADMUX has been pre-set in boot())
    while (bit_is_set(ADCSRA, ADSC)) { } // wait for conversion complete

    seed = ADC;
    power_adc_disable(); // ADC off
    seed ^= (uint16_t)micros();

}

uint16_t fast_random() {
    seed = seed * 25173u + 13849u;
    return seed;
}