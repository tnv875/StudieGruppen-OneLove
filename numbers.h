/* Do NOT use C control flow such as if/else statements in this file*/
#include "bits.h"

struct bits8 bits8_from_int(unsigned int x) {
    assert(x < 256);
    struct bits8 c;
    // TODO
    c.b7 = bit_from_int(x / 128);
    c.b0 = bit_from_int(x % 2);
    return c;
}

unsigned int bits8_to_int(struct bits8 x) {

}
// Prints the bits of bits8 in conventional order with no trailing newline
void bits8_print(struct bits8 v) {
    printf("%d%d%d%d %d%d%d%d", 
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
