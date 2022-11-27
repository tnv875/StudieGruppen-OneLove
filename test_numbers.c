#include "numbers.h"

int main() {
    // TASK 2

    // bits8 structs used for tests
    struct bits8 byte1 = bits8_from_int(1);    
    struct bits8 byte2 = bits8_from_int(255);
    struct bits8 byte3 = bits8_from_int(133);
    struct bits8 byte4 = bits8_from_int(5);

    // get_bit
    assert(get_bit(2,1) == 1);
    assert(get_bit(2,0) == 0);
    assert(get_bit(4,2) == 1);
    assert(get_bit(5,2) == 1);
    assert(get_bit(5,0) == 1);
    
    // bits8_from_int
    assert(
            byte1.b7.v == false
        &&  byte1.b6.v == false
        &&  byte1.b5.v == false
        &&  byte1.b4.v == false
        &&  byte1.b3.v == false
        &&  byte1.b2.v == false
        &&  byte1.b1.v == false
        &&  byte1.b0.v == true
    );

    assert(
            byte2.b7.v == true
        &&  byte2.b6.v == true
        &&  byte2.b5.v == true
        &&  byte2.b4.v == true
        &&  byte2.b3.v == true
        &&  byte2.b2.v == true
        &&  byte2.b1.v == true
        &&  byte2.b0.v == true
    );

    assert(
            byte3.b7.v == true
        &&  byte3.b6.v == false
        &&  byte3.b5.v == false
        &&  byte3.b4.v == false
        &&  byte3.b3.v == false
        &&  byte3.b2.v == true
        &&  byte3.b1.v == false
        &&  byte3.b0.v == true
    );

    // set_bit
    assert(set_bit(2, 0) == 3);
    assert(set_bit(2, 1) == 2);
    assert(set_bit(2, 2) == 6);

    // bits8_to_int
    assert(bits8_to_int(byte1) == 1);
    assert(bits8_to_int(byte2) == 255);
    assert(bits8_to_int(byte3) == 133);

    // TASK 3
    // bits8_add
    assert(bits8_to_int(bits8_add(byte1, byte3)) == 134);
    assert(bits8_to_int(bits8_add(byte1, byte4)) == 6);

    int t0 = bits8_to_int(bits8_add(byte1, byte3));
    int t1 = bits8_to_int(bits8_add(byte1, byte4));
    printf("Test af bits8_add:\n");
    printf("1 + 133 = %d\n", t0);
    printf("1 + 5 = %d\n", t1);

    // 
}
