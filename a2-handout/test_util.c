#include "util.h"
#include "io.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


int main() {
  FILE *f_points = fopen("20_5.points", "r");
  assert(f_points != NULL);
  
  FILE *f_indexes = fopen("5_5.indexes", "r");
  assert(f_indexes != NULL);

  int n, d, k;

  const double* points = read_points(f_points, &n, &d);
  assert(points != NULL);

  const int* indexes = read_indexes(f_indexes, &n, &k); 
  assert(indexes != NULL);

  // init closest with -1 in each place
  int *closest = malloc(k * sizeof(int));
  for (int i = 0; i < k; i++) {
    closest[i] = -1;
  }

  // use 4th point as our query
  const double* query = &(points[4*d]);

  int candidate = indexes[4];  
  insert_if_closer(k, d, points, closest, query, candidate);
    
  printf("Hej\n");
}
