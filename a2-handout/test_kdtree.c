#include "util.h"
#include "io.h"
#include "kdtree.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


int main() {
  FILE *f_points = fopen("10_2.points", "r"); 
  assert(f_points != NULL);

  int n, d;

  int k = 2;

  const double* points = read_points(f_points, &n, &d);
  
  assert(points != NULL);

  // init closest with -1 in each place
  int *closest = malloc(k * sizeof(int));
  for (int i = 0; i < k; i++) {
    closest[i] = -1;
  }

  // use 0.1, 0.1 as our query
  double* query = malloc(d * sizeof(double));
  query[0] = 0.1;
  query[1] = 0.1;
  
//   printf("In test_kdtree before kdtree_create() is called\n");
//   for (int i=0; i<n; i++) {
//     printf("Point %d: %f\n", i, points[i*d]);
//   }
  struct kdtree *tree = kdtree_create(d, n, points);
  printf("Success! Adress of tree is: %p\n", (void *)tree);
  printf("%d", 3/2);
}
