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

// TODO
struct naive_data* mk_naive(struct record* rs, int n) {

  // mk_index_fn* data_index = malloc(sizeof(mk_index_fn));

  struct naive_data* data_n = malloc(sizeof(struct naive_data));

  data_n->rs = rs;
  data_n->n = n;
  
  return data_n;
}

// TODO
void free_naive(struct naive_data* data) {
  
  free(data->rs);
  free(data);

}

// TODO
const struct record* lookup_naive(struct naive_data *data, int64_t needle) {
  
  // Iterates through the record array and returns a (the adress of?) pointer to the record with the 
  // matching ID if it is found.

  // i < data->n ===> i has to be less than the number of records in the array
  for (int i = 0; i < data->n; i++) {
    if (data->rs[i].osm_id == needle) {
      return &(data->rs[i]);
    }
  }

  // If no matching record is found, the function returns NULL.
  return NULL;

}

int main(int argc, char** argv) {
  return id_query_loop(argc, argv,
                    (mk_index_fn)mk_naive,
                    (free_index_fn)free_naive,
                    (lookup_fn)lookup_naive);
}
