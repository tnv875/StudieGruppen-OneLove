#include "numbers.h"

int main() {
    struct bits8 byte1 = bits8_from_int(1);
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

    struct bits8 byte2 = bits8_from_int(255);
    assert(
            byte1.b7.v == true
        &&  byte1.b6.v == true
        &&  byte1.b5.v == true
        &&  byte1.b4.v == true
        &&  byte1.b3.v == true
        &&  byte1.b2.v == true
        &&  byte1.b1.v == true
        &&  byte1.b0.v == true
    );

    struct bits8 byte3 = bits8_from_int(133);
    assert(
            byte1.b7.v == true
        &&  byte1.b6.v == false
        &&  byte1.b5.v == false
        &&  byte1.b4.v == false
        &&  byte1.b3.v == false
        &&  byte1.b2.v == true
        &&  byte1.b1.v == false
        &&  byte1.b0.v == true
    );
}