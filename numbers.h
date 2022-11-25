/* Do NOT use C control flow such as if/else statements in this file*/
#include "bits.h"

unsigned int get_bit(unsigned int x, int i) {
    return x >> i & 1;
}

struct bits8 bits8_from_int(unsigned int x) {
    assert(x < 256);
    struct bits8 c;
    c.b0 = bit_from_int(get_bit(x, 0));
    c.b1 = bit_from_int(get_bit(x, 1));
    c.b2 = bit_from_int(get_bit(x, 2));
    c.b3 = bit_from_int(get_bit(x, 3));
    c.b4 = bit_from_int(get_bit(x, 4));
    c.b5 = bit_from_int(get_bit(x, 5));
    c.b6 = bit_from_int(get_bit(x, 6));
    c.b7 = bit_from_int(get_bit(x, 7));
    return c;
}

unsigned int set_bit(unsigned int x, int i) {
    return 1 << i | x;
}

unsigned int bits8_to_int(struct bits8 x) {
    unsigned int c = 0;
    c = c | (bit_to_int(x.b0) * set_bit(0,0));
    c = c | (bit_to_int(x.b1) * set_bit(0,1));
    c = c | (bit_to_int(x.b2) * set_bit(0,2));
    c = c | (bit_to_int(x.b3) * set_bit(0,3));
    c = c | (bit_to_int(x.b4) * set_bit(0,4));
    c = c | (bit_to_int(x.b5) * set_bit(0,5));
    c = c | (bit_to_int(x.b6) * set_bit(0,6));
    c = c | (bit_to_int(x.b7) * set_bit(0,7)); 
    return c;
}

// Prints the bits of bits8 in conventional order with no trailing newline
void bits8_print(struct bits8 v) {
    printf("%d%d%d%d %d%d%d%d ", 
        v.b7.v, 
        v.b6.v, 
        v.b5.v,
        v.b4.v, 
        v.b3.v, 
        v.b2.v, 
        v.b1.v, 
        v.b0.v
    );
}
