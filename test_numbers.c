#include "numbers.h"

int main() {
    // TASK 2

    // bits8 structs used for tests
    struct bits8 byte1 = bits8_from_int(1);    
    struct bits8 byte3 = bits8_from_int(3);
    struct bits8 byte5 = bits8_from_int(5);
    struct bits8 byte133 = bits8_from_int(133);
    struct bits8 byte255 = bits8_from_int(255);

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
            byte255.b7.v == true
        &&  byte255.b6.v == true
        &&  byte255.b5.v == true
        &&  byte255.b4.v == true
        &&  byte255.b3.v == true
        &&  byte255.b2.v == true
        &&  byte255.b1.v == true
        &&  byte255.b0.v == true
    );

    assert(
            byte133.b7.v == true
        &&  byte133.b6.v == false
        &&  byte133.b5.v == false
        &&  byte133.b4.v == false
        &&  byte133.b3.v == false
        &&  byte133.b2.v == true
        &&  byte133.b1.v == false
        &&  byte133.b0.v == true
    );

    // set_bit 
    assert(set_bit(2, 0) == 3);
    assert(set_bit(2, 1) == 2); 
    assert(set_bit(2, 2) == 6);  

    // bits8_to_int
    assert(bits8_to_int(byte1) == 1);
    printf("%d", bits8_to_int(byte255)); 
    assert(bits8_to_int(byte255) == 255);
    assert(bits8_to_int(byte133) == 133);

    // TASK 3
    // bits8_add
    assert(bits8_to_int(bits8_add(byte1, byte133)) == 134);
    assert(bits8_to_int(bits8_add(byte1, byte5)) == 6);

    int t0 = bits8_to_int(bits8_add(byte1, byte133));
    int t1 = bits8_to_int(bits8_add(byte1, byte5));
    printf("Test af bits8_add:\n");
    printf("1 + 133 = %d\n", t0);
    printf("1 + 5 = %d\n", t1); 

    // TASK 4
    // bits8_negate()
    assert(bits8_to_int(bits8_negate(byte1)) == 255); // U255 = -1 TC
    printf("Test af bits8_negate");
    int t2 = bits8_to_int(bits8_negate(byte1));
    printf("-1 * 1 = %d\n", t2);

    // TASK 5
    // bits8_mul()
    bits8_print(bits8_mul(byte5, byte3)); 
    assert(bits8_to_int(bits8_mul(byte5, byte3)) == 15);
    assert(bits8_to_int(bits8_mul(byte1, byte5)) == 5); 
}
