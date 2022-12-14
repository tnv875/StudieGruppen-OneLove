#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

struct bit {
  bool v;
};

struct bit bit_from_int(unsigned int x) {
  assert(x < 2);
  struct bit b;
  if (x == 1) {
    b.v = true;
  } else {
    b.v = false;
  }
  return b;
}

unsigned int bit_to_int(struct bit b) {
  unsigned int x;
  if (b.v) {
    x = 1;
  } else {
    x = 0;
  }
  return x; 
}

void bit_print(struct bit b) {
  if (b.v) {printf("1\n");}
  else {printf("0\n");}
}

struct bit bit_not(struct bit a) {
  struct bit b;
  if  (a.v) { b.v = false;} 
  else      { b.v = true;}
  return b;
}

struct bit bit_and(struct bit a, struct bit b) {
  struct bit c;
  if (a.v & b.v) {
    c.v = true;
  } else {
    c.v = false;
  }
  return c;
}

struct bit bit_or(struct bit a, struct bit b) {
  struct bit c;
  if (a.v | b.v) {
    c.v = true;
  } else {
    c.v = false;
  }
  return c;
}

struct bit bit_xor(struct bit a, struct bit b) {
  struct bit c;
  if (a.v ^ b.v) {
    c.v = true;
  } else {
    c.v = false;
  }
  return c;
}
