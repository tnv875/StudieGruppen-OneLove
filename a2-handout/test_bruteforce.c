#include "bruteforce.h"
#include "util.h"
#include "io.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

int main() {
  FILE *f_points = fopen("10_2.points", "r"); 
  assert(f_points != NULL);

  // Initialize 'n' and 'd'
  int n, d;
  
  const double* points = read_points(f_points, &n, &d);
  assert(points != NULL);

  // Use 3rd point as our query in the test
  const double* query = &(points[3*d]);
  
  // Print result of bruteforce knn function
  int k = 3; //k
  int z = 2; //d
  int t = 10; //n

  //knn(k, z, t, points, query)[0]); 
  // Print the array using a for loop
  printf("The indexes of the array are: [");
  for (int i = 0; i < k; i++) {
    printf("%d, ", knn(k, z, t, points, query)[i]);
  }
  printf("]\n");

  int* k_nn = knn(k, z, t, points, query);
  
  // distance to candidate from query, z = d
  for (int j = 0; j < k; j++) {
    double dist_candidate = distance(z, query, &(points[k_nn[j] * z])); 
    printf("\nCandidate %d: dist_candidate: %f", k_nn[j], dist_candidate);
  } 
  printf("\n");
}
