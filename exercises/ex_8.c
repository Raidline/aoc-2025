#include "ex_8.h"
#include "../core/utils.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct point {
  long x;
  long y;
  long z;
  int index;
} point;

typedef struct node {
  struct point *left;
  struct point *right;
  double distance;
} node;

typedef struct point_group {
  struct point *p;
  int group;
} point_group;

void debug_groups(point_group groups[], int len) {
  for (int i = 0; i < len; i++) {
    printf("point : (%li,%li,%li) has group : [%d]\n", groups[i].p->x,
           groups[i].p->y, groups[i].p->z, groups[i].group);
  }
}

void debug_all_distances(node **distances, int len) {
  for (int i = 0; i < len; i++) {
    node *n = distances[i];

    printf("distance between :(%li,%li,%li) & (%li,%li,%li) -> %f\n",
           n->left->x, n->left->y, n->left->z, n->right->x, n->right->y,
           n->right->z, n->distance);
  }
}

void debug_top_distances(node **distances, int len) {
  for (int i = 0; i < len; i++) {
    node *n = distances[i];
    printf("top distance between :(%li,%li,%li) & (%li,%li,%li) -> %f\n",
           n->left->x, n->left->y, n->left->z, n->right->x, n->right->y,
           n->right->z, n->distance);
  }
}

void free_all_points(node *nodes[], int len) {
  for (int i = 0; i < len; i++) {
    free(nodes[i]->left);
    free(nodes[i]->right);
  }

  free(nodes);
}

long get_value(char *line, int line_len, int idx) {
  int len = line_len - idx;
  char number[len];
  strncpy(number, line + idx, len);

  return to_long(number);
}

point *extract_point(line_string *line, int i) {
  int j = 0;
  int x_idx = index_of(line->array_ptr, ',', j, line->str_len);
  long x_value = get_value(line->array_ptr, line->str_len, j);
  j = x_idx + 1;
  int y_idx = index_of(line->array_ptr, ',', j, line->str_len);
  long y_value = get_value(line->array_ptr, line->str_len, j);
  j = y_idx + 1;
  long z_value = get_value(line->array_ptr, line->str_len, j);

  point *p = malloc(sizeof(point));
  p->x = x_value;
  p->y = y_value;
  p->z = z_value;
  p->index = i;

  return p;
}

double calculate_distance(point *p1, point *p2) {
  double dx = p1->x - p2->x;
  double dy = p1->y - p2->y;
  double dz = p1->z - p2->z;

  // Apply the distance formula: sqrt(dx^2 + dy^2 + dz^2)
  return sqrt(pow(dx, 2) + pow(dy, 2) + pow(dz, 2));
}

int compare_distances(const void *a, const void *b) {
  const node *ra = *(node *const *)a;
  const node *rb = *(node *const *)b;

  if (ra->distance < rb->distance) {
    return -1;
  }
  if (ra->distance > rb->distance) {
    return 1;
  }
  return 0;
}

long ex_8(array_string *result) {

  node **nodes = malloc(result->length * sizeof(node *));
  node **top_diffs = malloc(10 * sizeof(node *)); // update for real example
  point_group groups[result->length];

  for (int i = 0; i < result->length - 1; i++) {
    line_string *line = result->lines[i];
    point *first = extract_point(line, i);
    point_group pg = {first, 0};
    groups[i] = pg;
    for (int j = i + 1; j < result->length; j++) {
      point *second = extract_point(result->lines[j], j);

      node *n = malloc(sizeof(node));
      n->left = first;
      n->right = second;
      n->distance = calculate_distance(first, second);

      nodes[i] = n;
    }
  }
  debug_all_distances(nodes, result->length);

  // qsort(nodes, result->length, sizeof(node *), compare_distances);

  for (int i = 0; i < 10; i++) {
    node *cpy = malloc(sizeof(node));
    cpy->distance = nodes[i]->distance;
    cpy->left = nodes[i]->left;
    cpy->right = nodes[i]->right;
    top_diffs[i] = cpy;

    free(cpy);
  }

  debug_groups(groups, result->length);

  debug_top_distances(top_diffs, 10);

  free_all_points(top_diffs, result->length);

  return 0;
}