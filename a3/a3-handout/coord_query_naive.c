#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <stdint.h>
#include <errno.h>
#include <assert.h>
#include <math.h>

#include "record.h"
#include "coord_query.h"

struct naive_data {
  struct record *rs;
  int n;
};

struct naive_data* mk_naive(struct record* rs, int n) {
  struct naive_data* data = malloc(sizeof(struct naive_data));
  data->rs = rs;
  data->n = n;
  return data;
}


void free_naive(struct naive_data* data) {
  free(data->rs);
  free(data);
}

double distance(double x_lon, double x_lat, double y_lon, double y_lat) { 
  return sqrt(pow((x_lon - y_lon), 2) + pow((x_lat - y_lat), 2));
}


const struct record* lookup_naive(struct naive_data *data, double lon, double lat) {

  double closest_dist = INFINITY; // index of closest record encountered so far
  int closest_i = -1; // index of closest record encountered so far

  for (int i = 0; i < data->n; i++) {
    double dist = distance(lon, lat, data->rs[i].lon, data->rs[i].lat);
    if (dist < closest_dist) {
      closest_dist = dist;
      closest_i = i;
    }
  }
  return &data->rs[closest_i];
}

int main(int argc, char** argv) {
  return coord_query_loop(argc, argv,
                          (mk_index_fn)mk_naive,
                          (free_index_fn)free_naive,
                          (lookup_fn)lookup_naive);
}
