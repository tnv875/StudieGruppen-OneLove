#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <stdint.h>
#include <errno.h>
#include <assert.h>

#include "record.h"
#include "id_query.h"

// "custom" struct
struct index_record {
  int64_t osm_id;
  const struct record *record;
};

// Has a pointer to an array of records (*irs) and an integer (n) 
// representing the number of records in the array.
struct indexed_data {
  struct index_record *irs;
  int n;
};

struct indexed_data* mk_indexed(struct record* rs, int n) {
  struct indexed_data* data = malloc(sizeof(struct indexed_data));
  
  data->irs->record = rs;
  data->irs->osm_id = rs[n]->osm_id;
  
  return data;
}


void free_indexed(struct indexed_data* data) {
  free(data);
}

const struct record* lookup_indexed(struct indexed_data *data, int64_t needle){
  for (int i = 0; i < (data->n); i++) {
    if (data->irs->osm_id == needle) {
      return data->irs->record;
    }
  }

  // If no matching record is found, the function returns NULL.
  return NULL;
  
}

// copied-ish from id_query_naive.c
int main(int argc, char** argv) {
  return id_query_loop(argc, argv,
                    (mk_index_fn)mk_indexed,
                    (free_index_fn)free_indexed,
                    (lookup_fn)lookup_indexed);
}
