#include "numbers.h"

int main() {

    // bits8 structs used for tests
    struct bits8 byte1 = bits8_from_int(1);     
    struct bits8 byte3 = bits8_from_int(3);
    struct bits8 byte5 = bits8_from_int(5);
    struct bits8 byte18 = bits8_from_int(18);
    struct bits8 byte133 = bits8_from_int(133);
    struct bits8 byte255 = bits8_from_int(255);


    // TASK 2

    // get_bit
    assert(get_bit(2,1) == 1);
    assert(get_bit(2,0) == 0);
    assert(get_bit(4,2) == 1);
    assert(get_bit(5,2) == 1);
    assert(get_bit(5,0) == 1);

    printf("\nA few printed tests of get_bit:\n");
    printf("Expected return: get_bit(2,1) = 1 –––> got: get_bit(2,1) = %d\n", get_bit(2,1));
    printf("Expected return: get_bit(5,2) = 1 –––> got: get_bit(5,2) = %d\n", get_bit(5,2));
    printf("Expected return: get_bit(2,2) = 0 –––> got: get_bit(2,2) = %d\n", get_bit(2,2));

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
            byte133.b7.v == true
        &&  byte133.b6.v == false
        &&  byte133.b5.v == false
        &&  byte133.b4.v == false
        &&  byte133.b3.v == false
        &&  byte133.b2.v == true
        &&  byte133.b1.v == false
        &&  byte133.b0.v == true
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

    printf("\nA few printed tests of bites8_from_int:\n");
    printf("Expected return: byte1 = 0000 0001   –––> got: "); bits8_print(byte1); 
    printf("\nExpected return: byte133 = 1000 0101 –––> got: "); bits8_print(byte133);
    printf("\nExpected return: byte255 = 1111 1111 –––> got: "); bits8_print(byte255);

    // set_bit 
    assert(set_bit(2, 0) == 3);
    assert(set_bit(2, 1) == 2); 
    assert(set_bit(2, 2) == 6);  

    printf("\n\nA few printed tests of set_bit:\n");
    printf("Expected return: set_bit(2,0) = 3 –––> got: set_bit(2,0) = %d\n", set_bit(2,0));
    printf("Expected return: set_bit(2,1) = 2 –––> got: set_bit(2,1) = %d\n", set_bit(2,1));
    printf("Expected return: set_bit(2,2) = 6 –––> got: set_bit(2,2) = %d\n", set_bit(2,2));

    // bits8_to_int
    assert(bits8_to_int(byte1) == 1);
    assert(bits8_to_int(byte133) == 133);
    assert(bits8_to_int(byte255) == 255);

    printf("\nA few printed tests of bits8_to_int:\n");
    printf("Expected return: byte1 = 1     –––> got: %d\n", bits8_to_int(byte1)); 
    printf("Expected return: byte133 = 133 –––> got: %d\n", bits8_to_int(byte133)); 
    printf("Expected return: byte255 = 255 –––> got: %d\n", bits8_to_int(byte255)); 


    // TASK 3

    // bits8_add
    assert(bits8_to_int(bits8_add(byte1, byte5)) == 6);
    assert(bits8_to_int(bits8_add(byte18, byte3)) == 21);
    assert(bits8_to_int(bits8_add(byte1, byte133)) == 134);

    printf("\nA few printed tests of bits8_add:\n");
    printf("Expected return: byte1 + byte5 = 6     –––> got: %d + %d = %d\n", bits8_to_int(byte1), bits8_to_int(byte5), bits8_to_int(bits8_add(byte1, byte5))); 
    printf("Expected return: byte18 + byte3 = 21   –––> got: %d + %d = %d\n", bits8_to_int(byte18), bits8_to_int(byte3), bits8_to_int(bits8_add(byte18, byte3))); 
    printf("Expected return: byte1 + byte133 = 134 –––> got: %d + %d = %d\n", bits8_to_int(byte1), bits8_to_int(byte133), bits8_to_int(bits8_add(byte1, byte133))); 

    // TASK 4

    // bits8_negate()
    assert(bits8_to_int(bits8_negate(byte1)) == 255); // U255 = -1 (TC)
    assert(bits8_to_int(bits8_negate(byte3)) == 253);
    assert(bits8_to_int(bits8_negate(byte133)) == 123);

    printf("\n\nA few printed tests of bits8_negate:\n");
    printf("Expected return: -1 * 1 = -1 = 255 (TC)     –––> got: %d\n", bits8_to_int(bits8_negate(byte1))); 
    printf("Expected return: -1 * 3 = -3 = 253 (TC)     –––> got: %d\n", bits8_to_int(bits8_negate(byte3))); 
    printf("Expected return: -1 * 133 = -133 = 123 (TC) –––> got: %d\n", bits8_to_int(bits8_negate(byte133)));


    // TASK 5

    // bits8_mul()
    assert(bits8_to_int(bits8_mul(byte5, byte3)) == 15);
    assert(bits8_to_int(bits8_mul(byte1, byte5)) == 5); 
    assert(bits8_to_int(bits8_mul(byte3, byte18)) == 54);


    printf("\nA few printed tests of bits8_mul:\n");
    printf("Expected return: 5 * 3 = 15   –––> got: %d * %d = %d = ", bits8_to_int(byte5), bits8_to_int(byte3), 
    bits8_to_int(bits8_mul(byte5, byte3))); bits8_print(bits8_mul(byte5, byte3));
    printf("\nExpected return: 1 * 5 = 5    –––> got: %d * %d = %d = ", bits8_to_int(byte1), bits8_to_int(byte5), 
    bits8_to_int(bits8_mul(byte1, byte5))); bits8_print(bits8_mul(byte1, byte5));
    printf("\nExpected return: 3 * 18 = 54  –––> got: %d * %d = %d = ", bits8_to_int(byte3), bits8_to_int(byte18), 
    bits8_to_int(bits8_mul(byte3, byte18))); bits8_print(bits8_mul(byte3, byte18));


    // Test concerning the "Answers for questions"-section:
    
    // Does bits8\_add() provide “correct” results if you pass in negative numbers in two’s complement representation?
    printf("\n\nTesting if bits8_add provide “correct” results if you pass in negative numbers in twos complement representation:\n");
    printf("Expected return: byte2 - byte6 (TC) = -4       –––> got: %d + %d (TC) = %d = ", bits8_to_int(bits8_from_int(2)), bits8_to_int(bits8_from_int(250)), bits8_to_int(bits8_add(bits8_from_int(2), bits8_from_int(250)))); 
    bits8_print(bits8_add(bits8_from_int(2), bits8_from_int(250))); printf(" : which equals -4 in TC");
    printf("\nExpected return: byte10 - byte12 (TC) = -2     –––> got: %d + %d (TC) = %d = ", bits8_to_int(bits8_from_int(10)), bits8_to_int(bits8_from_int(244)), bits8_to_int(bits8_add(bits8_from_int(10), bits8_from_int(244)))); 
    bits8_print(bits8_add(bits8_from_int(10), bits8_from_int(244))); printf(" : which equals -2 in TC");
    printf("\nExpected return: byte10 - byte8 (TC) = 2       –––> got: %d + %d (TC) = %d = ", bits8_to_int(bits8_from_int(10)), bits8_to_int(bits8_from_int(248)), bits8_to_int(bits8_add(bits8_from_int(10), bits8_from_int(248)))); 
    bits8_print(bits8_add(bits8_from_int(10), bits8_from_int(248))); printf(" : works correctly due to overflowing");
    printf("\nExpected return: byte133 - byte123 (TC) = 10   –––> got: %d + %d (TC) = %d = ", bits8_to_int(bits8_from_int(133)), bits8_to_int(bits8_from_int(133)), bits8_to_int(bits8_add(bits8_from_int(133), bits8_from_int(133)))); 
    bits8_print(bits8_add(bits8_from_int(133), bits8_from_int(133))); printf(" : works correctly due to overflowing (ex. from comments in pdf)");

    // How would you implement a function bits8_sub() for subtracting 8-bit numbers?
    printf("\n\nTesting the way we would implement bits8_sub:\n");
    printf("Expected return: byte5 - byte1 = 4      –––> got: %d\n", bits8_to_int(bits8_add(byte5, bits8_negate(byte1)))); 
    printf("Expected return: byte1 - byte5 = -4     –––> got: %d (equals -4 in TC, i.e. 1111 1100)\n", bits8_to_int(bits8_add(byte1, bits8_negate(byte5)))); 
    printf("Expected return: byte133 - byte18 = 115 –––> got: %d\n", bits8_to_int(bits8_add(byte133, bits8_negate(byte18))));
    
}

