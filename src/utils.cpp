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

//for (int x = 0x80; x <= 0xff; x++) {
//    recip_tab[x - 0x80] = 0x7fffff / x;
//    printf(x % 8 ? "0x%x, " : "\n    0x%x, ", recip_tab[x - 0x80]);
//}
const uint16_t recip_tab[128] PROGMEM = {
    0xffff, 0xfe03, 0xfc0f, 0xfa23, 0xf83e, 0xf660, 0xf489, 0xf2b9,
    0xf0f0, 0xef2e, 0xed73, 0xebbd, 0xea0e, 0xe865, 0xe6c2, 0xe525,
    0xe38e, 0xe1fc, 0xe070, 0xdee9, 0xdd67, 0xdbeb, 0xda74, 0xd901,
    0xd794, 0xd62b, 0xd4c7, 0xd368, 0xd20d, 0xd0b6, 0xcf64, 0xce16,
    0xcccc, 0xcb87, 0xca45, 0xc907, 0xc7ce, 0xc698, 0xc565, 0xc437,
    0xc30c, 0xc1e4, 0xc0c0, 0xbfa0, 0xbe82, 0xbd69, 0xbc52, 0xbb3e,
    0xba2e, 0xb921, 0xb817, 0xb70f, 0xb60b, 0xb509, 0xb40b, 0xb30f,
    0xb216, 0xb11f, 0xb02c, 0xaf3a, 0xae4c, 0xad60, 0xac76, 0xab8f,
    0xaaaa, 0xa9c8, 0xa8e8, 0xa80a, 0xa72f, 0xa655, 0xa57e, 0xa4a9,
    0xa3d7, 0xa306, 0xa237, 0xa16b, 0xa0a0, 0x9fd8, 0x9f11, 0x9e4c,
    0x9d89, 0x9cc8, 0x9c09, 0x9b4c, 0x9a90, 0x99d7, 0x991f, 0x9868,
    0x97b4, 0x9701, 0x964f, 0x95a0, 0x94f2, 0x9445, 0x939a, 0x92f1,
    0x9249, 0x91a2, 0x90fd, 0x905a, 0x8fb8, 0x8f17, 0x8e78, 0x8dda,
    0x8d3d, 0x8ca2, 0x8c08, 0x8b70, 0x8ad8, 0x8a42, 0x89ae, 0x891a,
    0x8888, 0x87f7, 0x8767, 0x86d9, 0x864b, 0x85bf, 0x8534, 0x84a9,
    0x8421, 0x8399, 0x8312, 0x828c, 0x8208, 0x8184, 0x8102, 0x8080,
};

//for (i = 0; i < 96; i++) {
//    rsqrt_tab[i] = (uint8_t)(256.0 * sqrt(1.0 / (1.0 + (i + 0.5) / 32.0)) * 256.0 + 0.5);
//}
const uint8_t rsqrt_tab[96] PROGMEM = {
    0xfe, 0xfa, 0xf7, 0xf3, 0xf0, 0xec, 0xe9, 0xe6, 0xe4, 0xe1, 0xde, 0xdc,
    0xd9, 0xd7, 0xd4, 0xd2, 0xd0, 0xce, 0xcc, 0xca, 0xc8, 0xc6, 0xc4, 0xc2,
    0xc1, 0xbf, 0xbd, 0xbc, 0xba, 0xb9, 0xb7, 0xb6, 0xb4, 0xb3, 0xb2, 0xb0,
    0xaf, 0xae, 0xac, 0xab, 0xaa, 0xa9, 0xa8, 0xa7, 0xa6, 0xa5, 0xa3, 0xa2,
    0xa1, 0xa0, 0x9f, 0x9e, 0x9e, 0x9d, 0x9c, 0x9b, 0x9a, 0x99, 0x98, 0x97,
    0x97, 0x96, 0x95, 0x94, 0x93, 0x93, 0x92, 0x91, 0x90, 0x90, 0x8f, 0x8e,
    0x8e, 0x8d, 0x8c, 0x8c, 0x8b, 0x8a, 0x8a, 0x89, 0x89, 0x88, 0x87, 0x87,
    0x86, 0x86, 0x85, 0x84, 0x84, 0x83, 0x83, 0x82, 0x82, 0x81, 0x81, 0x80,
};

// Computes 1/sqrt(x) for x=[0,0xffff] in Q0
// x > 0 returns 1/sqrt(x) in Q16
// x = 0 returns 0xffff
// max error = 3ULP
uint16_t rsqrt(uint16_t x) {

    if (x <= 1) return 0xffff;

    // normalize
    uint8_t n = CLZ16(x) & 0xfe;    // even
    x <<= n;    // Q16

    // initial estimate
    uint8_t t = pgm_read_byte(&rsqrt_tab[(x >> 9) - 32]);   // Q8

    // Newton-Raphson iteration
    uint16_t s = t * t;
    uint16_t r = 0xc000 - (UMUL32(x, s) >> 16); // Q16
    r = UMUL32(r, t) >> 7;

    // undo normalize
    n = 7 - (n >> 1);
    r >>= n;

    return r;   // Q16
}

// Computes sin(pi*x) and cos(pi*x) for unsigned x=[ 0,2] in Q15
// max error = 2ULP                    or signed x=[-1,1] in Q15
void sincospi(uint16_t ux, int16_t* ps, int16_t* pc) {

    // reduce to quadrant, and x in [-0.25,0.25]
    uint16_t q = (ux + 0x2000) & 0xc000;    // quadrant [0,3] in 2 MSBs
    ux <<= 2;                               // normalize to Q15
    if (ux == 0x8000) ux++;                 // saturate -32768, else x^2 overflows
    int16_t x = (int16_t)ux;

    int16_t xx = MUL15(x, x);

    // sin approx, as odd polynomial
    int16_t s = 81;
    s = MUL15(s, xx) - 2646;
    s = MUL15(s, xx) + 25736;
    s = MUL15(s, x);

    // cos approx, as even polynomial
    int16_t c = 510;
    c = MUL15(c, xx) - 10104;
    c = MUL15(c, xx) + 32767;

    // reconstruct using quadrant
    if (q & 0x8000) {
        s = -s;
        c = -c;
    }
    if (q & 0x4000) {
        int16_t t = s;
        s = c;
        c = -t;
    }
    *ps = s;
    *pc = c;
}

void init_fast_random_seed() {
    power_adc_enable(); // ADC on

    // do an ADC read from an unconnected input pin
    ADCSRA |= _BV(ADSC); // start conversion (ADMUX has been pre-set in boot())
    while (bit_is_set(ADCSRA, ADSC)) { } // wait for conversion complete

    seed = ADC;
    seed ^= (uint16_t)micros();

    power_adc_disable(); // ADC off
}

uint16_t fast_random() {
    seed = seed * 25173u + 13849u;
    return seed;
}