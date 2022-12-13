#include "bruteforce.h"
#include "util.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

int* knn(int k, int d, int n, const double *points, const double* query) {

  // Allocate memory for "closest" array
  int *arr = malloc(k*sizeof(int));
  
  // Iterate for each reference point, and populate "closest" array
  for (int i = 0; i < n; i++) {
    insert_if_closer(k, d, points, arr, query, i);
  }
  
  // indexes of the nearest neighbours to 'query' in 'points'.
  
  return arr;
}