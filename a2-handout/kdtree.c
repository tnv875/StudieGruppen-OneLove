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

// custom struct to allow passing of multiple arguments to compare_dim()
struct Arg {
  int axis;
  int d;
  const double *points;
};


// comparison function to be passed to quicksort
int compare_axis(const void* a, const void* b, void* arg) {
  
  // dereferencing
  // l: left index, r: right index in list of indexes to be sorted
  int l = *(int *)a; //converting void pointer to int pointer, then dereferencing to int
  // printf("l: %d\n", l);
  // printf("r: %d\n", l);
  int r = *(int *)b;
  int axis = ((struct Arg *)arg)->axis;
  const double* points = ((struct Arg *)arg)->points;
  int d = ((struct Arg *)arg)->d;

  double coord_l = points[(l * d) + axis];
  double coord_r = points[(r * d) + axis];
  // printf("coord_l: %f", coord_l);
  // printf("axis: %d\n", axis);
  // printf("point_l: %f\n", point_l);

  if (coord_l < coord_r) {
    return -1;
  } else {
    return 1;
  }
  // return (*(int *)a - *(int *)b);
}


struct node* kdtree_create_node(int d, const double *points,
                                int depth, int n, int *indexes) {
  struct node* no = malloc(sizeof(struct node));
  // pick axis to take median from, e.g. for d=2: y-axis or x-axis
  no->axis = depth % d;

  // create arg struct for passing arguments to sorting function
  struct Arg* arg = malloc(sizeof(struct Arg));
  // printf("In test_kdtree before kdtree_create() is called\n");
  // for (int i=0; i<n; i++) {
  //   printf("Point %d: %f\n", i, points[i*d]);
  // }
  arg->axis = no->axis;
  arg->points = points;
  arg->d = d;

  // sort indexes according to axis
  for (int i = 0; i<n; i++) {
    printf("indexes[%d]: %d\n", i, indexes[i]);
  }
  hpps_quicksort(indexes, n, sizeof(int), compare_axis, arg);
  free(arg);

  // pick median from sorted indexes
  // if n is even, this will be the higher of the two middle points. E.g:
  // n=6,  n/2=3, 
  // indexes[3] is then the higher of the two middle values, indexes[2] and indexes[3] 
  no->point_index = indexes[n/2];

  // recursively create left- and right nodes, using median index n/2 
  // to split indexes into left part and right part.
  if (n > 1) {
    no->left  = kdtree_create_node(d, points, depth+1, n/2, indexes);
    no->right = kdtree_create_node(d, points, depth+1, (n-1)/2, &(indexes[(n/2) + 1]));
  } else {
    no->left  = NULL;
    no->right = NULL;
  }

  // finally return address of node
  return no;
}

struct kdtree *kdtree_create(int d, int n, const double *points) {
  // printf("In kdtree right when kdtree_create() is called\n");
  // for (int i=0; i<n; i++) {
  //   printf("Point %d: %f\n", i, points[i*d]);
  // }
  struct kdtree *tree = malloc(sizeof(struct kdtree));
  tree->d = d;
  tree->points = points;
  // printf("In kdtree after struct kdtree *tree is initialised with points\n");
  // for (int i=0; i<n; i++) {
  //   printf("tree->point %d: %f\n", i, tree->points[i*d]);
  // }

  int *indexes = malloc(sizeof(int) * n);

  for (int i = 0; i < n; i++) {
    indexes[i] = i;
  }

  tree->root = kdtree_create_node(d, points, 0, n, indexes);

  free(indexes);

  return tree;
}

void kdtree_free_node(struct node *node) {
  // should we call recursively for left/right nodes?
  // I think we should:
  if (node->left != NULL) {
    kdtree_free_node(node->left);
    kdtree_free_node(node->right);
  }
  free(node);
}

void kdtree_free(struct kdtree *tree) {
  kdtree_free_node(tree->root);
  free(tree);
}



void kdtree_knn_node(const struct kdtree *tree, int k, const double* query, int *closest, double *radius, const struct node *node) {

  // Below I have tried to implement the pseudocode on p. 5 in assignment

  if (node == NULL) {
    return;
  }

  // Unpacking args
  // - for tree
  int d = tree->d;
  const double* points = tree->points;

  // - for node
  int axis = node->axis;
  int point_index = node->point_index;


  insert_if_closer(k, d, points, closest, query, point_index);

  //diff
  const double* node_point = &(points[point_index * d]);
  double diff = node_point[axis] - query[axis];


  // Update radius
  // closest[0] is index of furthest point currently in closest
  // as closest is maintained in sorted state by insert_if_closer()
  if (closest[0] != -1) {
    const double* furthest = &(points[closest[0] * d]);
    *radius = distance(d, furthest, query);
  }

  // fabs() returns absolute value of a double
  if (diff >= 0.0 || *radius > fabs(diff)) {
    kdtree_knn_node(tree, k, query, closest, radius, node->left);
  }
  if (diff <= 0.0 || *radius > fabs(diff)) {
    kdtree_knn_node(tree, k, query, closest, radius, node->right);
  }
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
