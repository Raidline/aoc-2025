#include "ex_8.h"
#include "../core/utils.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int MAX_GROUPS = 3;
const int MAX_LENGTH = 1000;

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

typedef struct group_aggregation {
  point_group group;
  int size;
} group_aggregation;

typedef enum { false, true } bool;

void debug_group_count(int group_sizes[], int len) {
  for (int i = 0; i < len; i++) {
    printf("Group : %d, has size : [%d]\n", i, group_sizes[i]);
  }
}

void debug_groups(point_group groups[], int len) {
  for (int i = 0; i < len; i++) {
    printf("point : (%li,%li,%li) in group : [%d]\n", groups[i].p->x,
           groups[i].p->y, groups[i].p->z, groups[i].group);
  }
}

void debug_all_distances(node **distances, int len) {
  for (int i = 0; i < len; i++) {
    node *n = distances[i];

    if (n == NULL) {
      printf("NULL node! at index:[%d]\n", i);
    } else {
      printf("distance between :([index:%d]:%li,%li,%li) & "
             "([index:%d]:%li,%li,%li) -> %f\n",
             n->left->index, n->left->x, n->left->y, n->left->z,
             n->right->index, n->right->x, n->right->y, n->right->z,
             n->distance);
    }
  }
}

void fill_bool_array(bool arr[], int len, bool value) {
  for (int i = 0; i < len; i++) {
    arr[i] = value;
  }
}

void free_nodes(node **nodes, int len) {
  for (int i = 0; i < len; i++) {
    free(nodes[i]);
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

int compare_group_size(const void *a, const void *b) {
  const int ra = *(int const *)a;
  const int rb = *(int const *)b;

  if (ra < rb) {
    return 1;
  }
  if (ra > rb) {
    return -1;
  }
  return 0;
}

void update_affected_neighbours(point_group groups[], int len,
                                int affected_group, int new_group,
                                long groups_sizes[]) {

  for (int i = 0; i < len; i++) {

    point_group pg = groups[i];

    if (pg.group == affected_group) {
      groups_sizes[groups[i].group] = groups_sizes[groups[i].group] - 1;
      groups[i].group = new_group;
      groups_sizes[new_group] = groups_sizes[new_group] + 1;
    }
  }
}

long ex_8(array_string *result) {

  int cap = result->length;
  node **nodes = malloc(cap * sizeof(node *));
  point_group groups[result->length];
  int nodes_len = 0;

  for (int i = 0; i < result->length; i++) {
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

      nodes[nodes_len++] = n;

      if (nodes_len == cap) {
        cap = cap * 2;

        nodes = realloc(nodes, cap * sizeof(node *));
      }
    }
  }
  // debug_all_distances(nodes, nodes_len);

  qsort(nodes, nodes_len, sizeof(node *), compare_distances);

  nodes = realloc(nodes, MAX_LENGTH * sizeof(node *));

  //printf("Looking at top 10 distances\n");

  //debug_all_distances(nodes, MAX_LENGTH);

  int group_counter = 1;
  long *groups_sizes = calloc(MAX_LENGTH, sizeof(int));

  // debug_groups(groups, result->length);

  for (int i = 0; i < MAX_LENGTH; i++) {
    node *n = nodes[i];
    point *pl = n->left;
    point *pr = n->right;

    int group_l = groups[pl->index].group;
    int group_r = groups[pr->index].group;

    if (group_l == 0 && group_r == 0) {
      groups[pl->index].group = group_counter;
      groups[pr->index].group = group_counter;

      groups_sizes[group_counter] = 2;
      group_counter++;

    } else if (group_l == group_r) {
      // ignore
      continue;
    } else if (group_l != 0 && group_r != 0) {

      // put in the same group
      if (group_l < group_r) {
        groups_sizes[groups[pl->index].group] =
            groups_sizes[groups[pl->index].group] + 1;

        groups_sizes[groups[pr->index].group] =
            groups_sizes[groups[pr->index].group] - 1;

        groups[pl->index].group = group_l;
        groups[pr->index].group = group_l;
        // both nodes are seen

        update_affected_neighbours(groups, result->length, group_r, group_l,
                                   groups_sizes);
      } else {
        groups_sizes[groups[pr->index].group] =
            groups_sizes[groups[pr->index].group] + 1;

        groups_sizes[groups[pl->index].group] =
            groups_sizes[groups[pl->index].group] - 1;

        groups[pl->index].group = group_r;
        groups[pr->index].group = group_r;

        // both nodes are seen

        update_affected_neighbours(groups, result->length, group_l, group_r,
                                   groups_sizes);
      }
    } else {
      // printf("current indexes: [%d,%d], for groups : [%d,%d]\n", pl->index,
      //        pr->index, group_index_l, group_index_r);
      // one of them is not assigned
      int max_index = MAX(group_l, group_r);
      // this will always be the correct group as the other is 0
      groups[pl->index].group = max_index;
      groups[pr->index].group = max_index;

      groups_sizes[max_index] = groups_sizes[max_index] + 1;
    }
  }

  // debug_groups(groups, result->length);

  qsort(groups_sizes, MAX_LENGTH, sizeof(long), compare_group_size);

  long multipler = 1;

  for (int i = 0; i < MAX_GROUPS; i++) {
    multipler *= groups_sizes[i];
  }

  // debug_group_count(groups_sizes, MAX_LENGTH);
  free_nodes(nodes, MAX_LENGTH);

  return multipler;
}