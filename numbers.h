/* Do NOT use C control flow such as if/else statements in this file */
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
    struct bit s;
    struct bit c;
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
    t.b0 = ; 
    t.b1 = ;
    t.b2 = ;
    t.b3 = ;
    t.b4 = ;
    t.b5 = ;
    t.b6 = ;
    t.b7 = ;

    return t;
}
 
