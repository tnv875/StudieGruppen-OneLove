#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <time.h>

#define MAX_SIZE 100

double* read_points(FILE *f, int* n_out, int *d_out) {
  assert(f != NULL);
  size_t n = fread(n_out, sizeof(n_out), MAX_SIZE, f);
  size_t d = fread(d_out, sizeof(d_out), MAX_SIZE, f);
  double* ndmatrix = malloc(sizeof(n)*sizeof(d)*sizeof(double));
  return ndmatrix;
  free(ndmatrix);
}

int main(int argc, char** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <file>\n", argv[0]);
    exit(1);
  }

  FILE *f = fopen(argv[1], "r");
  assert(f != NULL);

  int n, d;
  double *points = read_points(f, &n, &d);
  assert(points != NULL);

  for (int i = 0; i < n; i++) {
    printf("Point %d: ", i);
    for (int j = 0; j < d; j++) {
      printf("%f ", points[i*d+j]);
    }
    printf("\n");
  }

  free(points);
}

/*
int* read_indexes(FILE *f, int *n_out, int *k_out) {
  assert(0);
}


int write_points(FILE *f, int32_t n, int32_t d, double *data) {
  assert(0);
}

int write_indexes(FILE *f, int32_t n, int32_t k, int *data) {
  assert(0);
}
*/
