#include "util.h"
#include <math.h>
#include <stdio.h>
#include <assert.h>

// √( ∑ (x[i]-y[i])² )
double distance(int d, const double *x, const double *y) {
  double s = 0;
  for (int i = 0; i < d; i++) {
    s += pow((x[i] - y[i]), 2);
  }
  return sqrt(s);
}

int insert_if_closer(int k, int d,
                     const double *points, int *closest, 
                     const double *query, int candidate) {

  // distance to candidate from query
  // !! TODO: Check if address is correct. We think it is! :)
  double dist_candidate = distance(d, query, &points[candidate]); 

  // furthest distance in closest seen so far
  double furthest = -1.0;
  int furthest_i = 0;

  // loop through all indexes in closest
  for (int i = 0; i < k; i++) {
    // if closest has empty spaces, trivially insert candidate and return 1
    if (closest[i] == -1) {
      closest[i] = candidate;
      return 1;
      
    // calculate distance between query and the i'th point 
    } else {
      int i_index = closest[i];
      const double i_point = points[i_index];
      double dist_i = distance(d, query, &i_point); //check address

      // check if we need to update furthest
      if (furthest < dist_i) {
        furthest = dist_i;
        furthest_i = i;
      }   
    }
  }

  // inserts candidate in 'closest' if the distance to the candidate is smaller
  // than furthest
  if (furthest > dist_candidate) {
    closest[furthest_i] = candidate;
    return 1;
  } else {
    return 0;
  }
}