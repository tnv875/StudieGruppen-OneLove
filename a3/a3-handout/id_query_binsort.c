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
struct sorted_data {
  struct index_record *irs;
  int n;
};


// Comparison function to compare id's of records
int compare(const void* a, const void* b) {
  struct index_record* l = (struct index_record*)a;
  struct index_record* r = (struct index_record*)b;

  if (l->osm_id < r->osm_id) {
    return -1;
  }
  return 1;

}


// Sorts data using among other things "qsort" and our implemented comparison function 
struct sorted_data* mk_binsort(struct record* rs, int n) {
  struct sorted_data* data = malloc(sizeof(struct sorted_data));

  data->irs = calloc(n, sizeof(struct index_record));
  data->n = n;

  for (int i=0; i < n; i++) {
    data->irs[i].osm_id = rs[i].osm_id; 
    data->irs[i].record = &rs[i]; 
  }

  qsort(data->irs, n, sizeof(struct index_record), &compare);

  return data;
}


void free_indexed(struct sorted_data* data) {
  free(data->irs);
  free(data);
}


// Binary search function to be used to lookup indexes below
int binarySearch(struct index_record* array, int x, int low, int high) {
  // Repeat until the pointers low and high meet each other
  while (low <= high) {
    int mid = low + (high - low) / 2;

    if (array[mid].osm_id == x)
      return mid;

    if (array[mid].osm_id < x)
      low = mid + 1;

    else
      high = mid - 1;
  }

  return -1;
}


const struct record* lookup_indexed(struct sorted_data *data, int64_t needle){
  int x = binarySearch(data->irs, 
                       needle, 
                       0,
                       data->n);
  if (x != -1) {
    return data->irs[x].record;
  }

  // If no match is found, the function returns NULL.
  return NULL;
}


// copied-ish from id_query_naive.c
int main(int argc, char** argv) {
  return id_query_loop(argc, argv,
                    (mk_index_fn)mk_binsort,
                    (free_index_fn)free_indexed,
                    (lookup_fn)lookup_indexed);
}
