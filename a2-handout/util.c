#include "util.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


// √( ∑ (x[i]-y[i])² )
double distance(int d, const double *x, const double *y) {
  double s = 0;
  for (int i = 0; i < d; i++) {
    s += pow((x[i] - y[i]), 2);
  }
  return sqrt(s);
}


// Auxillary function to sort first val in Closest 
// to correct position (requires Closest is empty or already sorted)
int sort_first(int *closest, int k, int d, const double *points,
                    const double *query) {
  double dist_candidate = distance(d, query, &(points[closest[0] * d]));
  double dist_comp;
  int tmp;
  for (int i=1; i<k; i++) {

    if (closest[i] == -1) {
      // swap
      tmp = closest[i-1];
      closest[i-1] = closest[i];
      closest[i] = tmp;
      continue;
    }
    // break if we are done
    dist_comp = distance(d, query, &(points[closest[i] * d]));
    if (dist_comp < dist_candidate) {
      return 0; // return now if we are done sorting
    }

    // swap if no other conditions are met
    tmp = closest[i-1];
    closest[i-1] = closest[i];
    closest[i] = tmp;
  }
  return 1; //if candidate is moved to the very end
}


int insert_if_closer(int k, int d,
                     const double *points, int *closest, 
                     const double *query, int candidate) {

  // if first spot is empty (closest has empty spaces)
  if (closest[0] == -1) {
    closest[0] = candidate;
    sort_first(closest, k, d, points, query);
    return 1;
  }

  // since Closest is already sorted in decending order, 
  // we only need to compare with first elm as that is furthest

  // distance to candidate from query
  double dist_candidate = distance(d, query, &(points[ candidate * d])); 
  double dist_furthest  = distance(d, query, &(points[closest[0] * d]));
  if (dist_candidate < dist_furthest) {
    closest[0] = candidate;
    sort_first(closest, k, d, points, query);
    return 1;
  }
  
  return 0;
}
