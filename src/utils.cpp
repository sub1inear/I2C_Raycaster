#include "utils.h"

extern Arduboy2 arduboy;

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
//for (int x = 0x80; x <= 0xff; x++) {
//    recip_tab[x - 0x80] = 0x7fffff / x;
//    printf(x % 8 ? "0x%x, " : "\n    0x%x, ", recip_tab[x - 0x80]);
//}


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


// line rendering data

const uint8_t SET_MASK[8] = {
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80
};

const uint8_t YMASK0[8] = {
    0xff, 0xfe, 0xfc, 0xf8, 0xf0, 0xe0, 0xc0, 0x80
};
const uint8_t YMASK1[8] = {
    0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff
};

const uint16_t PATTERNS[4] = {
//  0x0000,
    0xaa00,
    0xaa55,
    0xff55,
    0xffff,
};

void draw_vline(uint8_t x, int16_t y0, int16_t y1, uint16_t pat) {
    if (y0 > y1) return;
    if (y1 < 0) return;
    if (y0 >= FBH) return;
    if (x >= FBW) return;

    uint8_t ty0 = (uint8_t)tmax<int16_t>(y0, 0);
    uint8_t ty1 = (uint8_t)tmin<int16_t>(y1, FBH - 1);

    uint8_t t0 = ty0 & 0xf8;
    uint8_t t1 = ty1 & 0xf8;
    uint8_t m0 = YMASK0[ty0 & 0x7];
    uint8_t m1 = YMASK1[ty1 & 0x7];

    uint8_t pattern = (x & 0x1) ? uint8_t(pat) : uint8_t(pat >> 8);

    uint8_t* p = &arduboy.sBuffer[t0 * (FBW / 8) + x];

    if (t0 == t1) {
        uint8_t m = m0 & m1;
        uint8_t tp = *p;
        tp |= (pattern & m);
        tp &= (pattern | ~m);
        *p = tp;
        return;
    }
    {
        uint8_t m = m0;
        uint8_t tp = *p;
        tp |= (pattern & m);
        tp &= (pattern | ~m);
        *p = tp;
        p += FBW;
    }
    for (int8_t t = t1 - t0 - 8; t > 0; t -= 8) {
        *p = pattern;
        p += FBW;
    }
    {
        uint8_t m = m1;
        uint8_t tp = *p;
        tp |= (pattern & m);
        tp &= (pattern | ~m);
        *p = tp;
    }
}