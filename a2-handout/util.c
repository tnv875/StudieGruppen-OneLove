#include "util.h"
#include "sort.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct Arg {
    const int d;
    const double *query;
};


// √( ∑ (x[i]-y[i])² )
double distance(int d, const double *x, const double *y) {
  double s = 0;
  for (int i = 0; i < d; i++) {
    s += pow((x[i] - y[i]), 2);
  }
  return sqrt(s);
}


// Auxilary function to lowest distance of node 0 and node 1 
// x: distance from node 0 to query
// y: distance from node 1 to query
// arg: array with [0] = &d, [1] = query
int compare(const double *x, const double *y, struct Arg *arg) {
  if (distance(arg->d, x, arg->query) < distance(arg->d, y, arg->query)) {
    return -1;
  }
  return 1;
}

void sort_descending(int *array, int k, int d, const double *query) {
  struct Arg arg = {d, query};

  // ugly stuff to allow passing the compare function to quicksort
  int (*f)(const void *, const void *, void *);
  f = &compare;

  hpps_quicksort(array, k, sizeof(double), f, &arg);
}


int insert_if_closer(int k, int d,
                     const double *points, int *closest, 
                     const double *query, int candidate) {

  // if first spot is empty (closest has empty spaces)
  if (closest[0] == -1) {
    closest[0] = candidate;
    sort_descending(closest, k, d, query);
    return 1;
  }

  // since Closest is sorted in decending order, we only need to compare with first elm

  // distance to candidate from query
  double dist_candidate = distance(d, query, &(points[ candidate * d])); 
  double dist_furthest  = distance(d, query, &(points[closest[0] * d]));
  if (dist_candidate < dist_furthest) {
    closest[0] = candidate;
    sort_descending(closest, k, d, query);
    return 1;
  }
  
  return 0;



  // OLD IMPLEMENTATION
  // // furthest distance in closest seen so far
  // double furthest = -1.0;
  // int furthest_i = 0;


  // // loop through all indexes in closest
  // for (int i = 0; i < k; i++) {
  //   // if closest has empty spaces, trivially insert candidate and return 1
  //   if (closest[i] == -1) {
  //     closest[i] = candidate;
  //     printf("Trivial :p\n");
  //     return 1;
      
  //   // calculate distance between query and the i'th point 
  //   } else {
  //     int i_index = closest[i];
  //     const double* i_point = &(points[i_index * d]);
  //     double dist_i = distance(d, query, i_point);

  //     // check if we need to update furthest
  //     if (furthest < dist_i) {
  //       furthest = dist_i;
  //       furthest_i = i;
  //     }   
  //   }
  // }

  // // inserts candidate in 'closest' if the distance to the candidate is smaller
  // // than furthest
  // if (furthest > dist_candidate) {
  //   closest[furthest_i] = candidate;
  //   return 1;
  // } else {
  //   return 0;
  // }
}
