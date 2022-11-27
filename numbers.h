#include "bits.h"


// TASK 2


// helper function - returns bit i from the integer x
unsigned int get_bit(unsigned int x, int i) {
    assert(x < 256);
    assert(i < 8);
    return x >> i & 1;
}

// Convert C integer to a bits8
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

// helper function - returns x but with the bit at position i set to 1
unsigned int set_bit(unsigned int x, int i) {
    assert(x < 256);
    assert(i < 8);
    return 1 << i | x;
    // x = 0001 0101
    // 1 = 0000 0001
    // i = 3
    // y = 0001 1101
}

// Convert a bits8 to a C integer
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


// TASK 3


// defines a new type to contain two bits
struct add_result {
    struct bit s; // sum
    struct bit c; // carry
};

// helper function - implements a full adder (function (1) and (2) from the assignment)
struct add_result bit_add(struct bit x, struct bit y, struct bit c) {
    struct add_result a;
    
    // calculating value of s
    a.s = bit_xor(c, bit_xor(x, y));

    // Calculating value of c
    a.c = bit_or(bit_and(x, y), bit_and((bit_or(x, y)), c)); 

    return a;
}

struct bits8 bits8_add(struct bits8 x, struct bits8 y) {
    struct bits8 t;
    struct bit c;
    c.v = 0;
    struct add_result b0 = bit_add(x.b0, y.b0, c);
    t.b0 = b0.s;
    struct add_result b1 = bit_add(x.b1, y.b1, b0.c);
    t.b1 = b1.s;
    struct add_result b2 = bit_add(x.b2, y.b2, b1.c);
    t.b2 = b2.s;
    struct add_result b3 = bit_add(x.b3, y.b3, b2.c);
    t.b3 = b3.s;
    struct add_result b4 = bit_add(x.b4, y.b4, b3.c);
    t.b4 = b4.s;
    struct add_result b5 = bit_add(x.b5, y.b5, b4.c);
    t.b5 = b5.s;
    struct add_result b6 = bit_add(x.b6, y.b6, b5.c);
    t.b6 = b6.s;
    struct add_result b7 = bit_add(x.b7, y.b7, b6.c);
    t.b7 = b7.s;
    return t;
}

struct bits8 bits8_negate(struct bits8 x) {
    x.b0 = bit_not(x.b0)
    x.b1 = bit_not(x.b1)
    x.b2 = bit_not(x.b2)
    x.b3 = bit_not(x.b3)
    x.b4 = bit_not(x.b4)
    x.b5 = bit_not(x.b5)
    x.b6 = bit_not(x.b6)
    x.b7 = bit_not(x.b7)
    x = bits8_add(x, bits8_from_int(1))
    return x;
}
