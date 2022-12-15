#include "kdtree.h"
#include "sort.h"
#include "util.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

struct node {
  int point_index;
  int axis;
  struct node *left;
  struct node *right;
};

struct kdtree {
  int d;
  const double *points;
  struct node* root;
};

// custom struct to allow passing of multiple arguments to compare 
struct Arg {
  int axis;
  int d;
  const double *points;
};

// comparison function to be passed to quicksort
double compare_dim(const void* a, const void* b, void* arg) {
  
  // dereferencing
  // l: left, r: right elm in array to be sorted
  int l = *(int *)a; //converting void pointer to int pointer, then dereferencing to int
  int r = *(int *)b;
  int axis = ((struct Arg *)arg)->axis;
  const double *points = ((struct Arg *)arg)->points;
  int d = ((struct Arg *)arg)->d;

  double coord_l = (&points)[l*d][axis];
  double coord_r = (&points)[r*d][axis];

  // if coord l is bigger: positive. Else: negative
  return (coord_l - coord_r);
}

// ugly stuff to allow passing function to quicksort
double (*f)(const void *, const void *, void *);
f = &compare_dim;

struct node* kdtree_create_node(int d, const double *points,
                                int depth, int n, int *indexes) {
  struct node no;
  // pick axis to take median from, e.g. for d=2: yzz or x
  no.axis = depth % d;

  // create arg struct for passing arguments to sorting function
  struct Arg arg;
  arg.axis = no.axis;
  arg.points = points;
  arg.d = d;

  // sort indexes according to axis
  hpps_quicksort(indexes, n, (d * sizeof(double)), f, &arg);
  
  // pick median from sorted indexes
  no.point_index = (&indexes)[n/2 * d];

  // recursively create left- and right nodes, using median index n/2 
  // to split indexes into left part and right part. Right part starts 
  // at median.
  if (n > 2) {
    no.left  = kdtree_create_node(d, points, depth+1, n/2, indexes);
    no.right = kdtree_create_node(d, points, depth+1, n/2, indexes[n/2]);
  }

  // finally return address of node
  return &no;
}

struct kdtree *kdtree_create(int d, int n, const double *points) {
  struct kdtree *tree = malloc(sizeof(struct kdtree));
  tree->d = d;
  tree->points = points;

  int *indexes = malloc(sizeof(int) * n);

  for (int i = 0; i < n; i++) {
    indexes[i] = i;
  }

  tree->root = kdtree_create_node(d, points, 0, n, indexes);

  free(indexes);

  return tree;
}

void kdtree_free_node(struct node *node) {
  free(node);
}

void kdtree_free(struct kdtree *tree) {
  kdtree_free_node(tree->root);
  free(tree);
}

void kdtree_knn_node(const struct kdtree *tree, int k, const double* query,
                     int *closest, double *radius,
                     const struct node *node) {
  assert(0);
}

int* kdtree_knn(const struct kdtree *tree, int k, const double* query) {
  int* closest = malloc(k * sizeof(int));
  double radius = INFINITY;

  for (int i = 0; i < k; i++) {
    closest[i] = -1;
  }

  kdtree_knn_node(tree, k, query, closest, &radius, tree->root);

  return closest;
}

void kdtree_svg_node(double scale, FILE *f, const struct kdtree *tree,
                     double x1, double y1, double x2, double y2,
                     const struct node *node) {
  if (node == NULL) {
    return;
  }

  double coord = tree->points[node->point_index*2+node->axis];
  if (node->axis == 0) {
    // Split the X axis, so vertical line.
    fprintf(f, "<line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" stroke-width=\"1\" stroke=\"black\" />\n",
            coord*scale, y1*scale, coord*scale, y2*scale);
    kdtree_svg_node(scale, f, tree,
                    x1, y1, coord, y2,
                    node->left);
    kdtree_svg_node(scale, f, tree,
                    coord, y1, x2, y2,
                    node->right);
  } else {
    // Split the Y axis, so horizontal line.
    fprintf(f, "<line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" stroke-width=\"1\" stroke=\"black\" />\n",
            x1*scale, coord*scale, x2*scale, coord*scale);
    kdtree_svg_node(scale, f, tree,
                    x1, y1, x2, coord,
                    node->left);
    kdtree_svg_node(scale, f, tree,
                    x1, coord, x2, y2,
                    node->right);
  }
}

void kdtree_svg(double scale, FILE* f, const struct kdtree *tree) {
  assert(tree->d == 2);
  kdtree_svg_node(scale, f, tree, 0, 0, 1, 1, tree->root);
}
