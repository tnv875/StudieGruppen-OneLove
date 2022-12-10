#include "util.h"
#include "io.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


int main() {
  FILE *f_points = fopen("10_2.points", "r"); 
  assert(f_points != NULL);
  
  FILE *f_indexes = fopen("10_2.indexes", "r");
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

  // use 7th point as our query
  const double* query = &(points[7*d]);
      
  insert_if_closer(k, d, points, closest, query, 3);
  insert_if_closer(k, d, points, closest, query, 2);
  insert_if_closer(k, d, points, closest, query, 1);

  // Print query
  printf("Our query location: %f %f\n\n", query[0], query[1]);

  // Print indexes in closest
  printf("Indexes in 'closest': \n");
  for (int i = 0; i < k; i++) {
    printf("%d\n", closest[i]);
  } 

  // For nice print
  printf("\n");
    
  // Print points in closest
  for (int i = 0; i < k; i++) {  
    int ind = d * closest[i]; // multiply by d to fix dimensions
    const double* point = &(points[ind]);
    printf("Point %d in closest: %f %f\n", i, point[0], point[1]);
  } 

  // Note to self: insert_if_closer correctly inserts points in empty
  // spaces, but from then on does not correctly evaluate whether to update
  // points
}
 