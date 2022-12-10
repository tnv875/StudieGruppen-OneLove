#include "util.h"
#include <math.h>
#include <stdio.h>
#include <assert.h>

// √( ∑ (x[i]-y[i])² )
double distance(int d, const double *x, const double *y) {
  int s = 0;
  for (int i = 0; i < d; i++) {
    s += pow((x[i] - y[i]), 2);
  }
  return sqrt(s);
}

int insert_if_closer(int k, int d,
                     const double *points, int *closest, const double *query,
                     int candidate) {
  assert(0);
}
