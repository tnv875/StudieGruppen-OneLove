#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <stdint.h>
#include <errno.h>
#include <assert.h>

#include "record.h"
#include "id_query.h"

// OBS struct made in advance!
// Has a pointer to an array of records (*rs) and an integer (n) 
// representing the number of records in the array.
struct naive_data {
  struct record *rs;
  int n;
};

// A function that produces an index, when called with an
// array of records and the size of the array.
struct naive_data* mk_naive(struct record* rs, int n) {

  struct naive_data* data = malloc(sizeof(struct naive_data));

  data->rs = rs;
  data->n = n;
  
  return data;
}

void free_naive(struct naive_data* data) {
  
  free(data);

}

// Look up an ID in an index produced by mk_index_fn.
const struct record* lookup_naive(struct naive_data *data, int64_t needle) {
  
  // Iterates through the record array and returns the adress of a pointer to the record with the 
  // matching ID if it is found.

  // i < data->n ===> i has to be less than the number of records in the array
  for (int i = 0; i < (data->n); i++) {
    if (data->rs[i].osm_id == needle) {
      return &(data->rs[i]);
    }
  }

  // If no matching record is found, the function returns NULL.
  return NULL;

}

// input in terminal has to be the osm_id
int main(int argc, char** argv) {
  return id_query_loop(argc, argv,
                    (mk_index_fn)mk_naive,
                    (free_index_fn)free_naive,
                    (lookup_fn)lookup_naive);
}
