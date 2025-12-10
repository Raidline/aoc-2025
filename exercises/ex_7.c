#include "ex_7.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { false, true } bool;

typedef enum { LEFT, RIGHT } Direction;

typedef struct node {
  int line;
  int col;
  struct node *left;
  struct node *right;
} node;

char **alloc_memory_for_debug_state(array_string *result) {
  char **debug_state = malloc(result->length * sizeof(char *));

  for (int i = 0; i < result->length; i++) {
    debug_state[i] = malloc(result->lines[i]->str_len * sizeof(char));
  }

  return debug_state;
}

void free_tree(node *node) {
  if (node == NULL) {
    return;
  }

  if (node->left == NULL && node->right == NULL) {
    free(node);
    return;
  }

  free_tree(node->left);
  free_tree(node->right);
}

void print_tree(node *node) {
  if (node == NULL) {
    return;
  }

  printf("line:[%d],col:[%d]\n", node->line, node->col);

  print_tree(node->left);
  print_tree(node->right);
}

void print_debug_state(char **state, int len, int col_len) {
  for (int i = 0; i < len; i++) {
    for (int j = 0; j < col_len; j++) {
      printf("%c", state[i][j]);
    }
    printf("\n");
  }
}

void free_debug_state(char **state, int len) {
  for (int i = 0; i < len; i++) {
    free(state[i]);
  }

  free(state);
}

bool location_found(line_string *line, int len, int needle_col) {
  bool location_found = false;
  for (int c = 0; c < len; c++) {
    if (c == needle_col && line->array_ptr[c] == '|') {
      location_found = true;
      break;
    }
  }

  return location_found;
}

node *find_node(node *node, int line, int col) {
  if (node == NULL) {
    return NULL;
  }

  printf("needle:[%d],col:[%d]-", line, col);
  printf("looking into node: line:[%d],col:[%d]\n", node->line, node->col);

  if (node->line == line && node->col == col) {
    return node;
  }

  node *nd = find_node(node->left, line, col);

  if (nd != NULL) {
    return nd;
  }

  return find_node(node->right, line, col);
}

node *create_node(int line, int col, node *left, node *right) {
  node *n = malloc(sizeof(node));
  n->line = line;
  n->col = col;
  n->left = left;
  n->left = right;
  return n;
}

void insert_into_tree(node *node, int line, int col, Direction dir) {
  switch (dir) {
  case LEFT: {
    node->left = create_node(line, col, NULL, NULL);
    break;
  }
  case RIGHT: {
    node->right = create_node(line, col, NULL, NULL);
  }
  }
}

long ex_7(array_string *result) {

  long count = 0;

  int times_splitted = 0;

  char **debug_state = alloc_memory_for_debug_state(result);
  node *parent;

  for (int column = 0; column < result->lines[0]->str_len; column++) {
    debug_state[0][column] = result->lines[0]->array_ptr[column];
    if (result->lines[0]->array_ptr[column] == 'S') {
      result->lines[1]->array_ptr[column] = '|';
      parent = create_node(1, column, NULL, NULL);
      break;
    }
  }

  for (int line = 1; line < result->length; line++) {
    line_string *columns = result->lines[line];

    for (int column = 0; column < columns->str_len; column++) {
      char ch = columns->array_ptr[column];
      debug_state[line][column] = ch;
      // line_string *top_line = result->lines[line - 1];
      //  bool has_top_beam = location_found(top_line, top_line->str_len,
      //  column);
      node *top_node = find_node(parent, line - 1, column);

      if (top_node != NULL && ch != '^') {
        columns->array_ptr[column] = '|';
        debug_state[line][column] = '|';

        if (column / 2 <= columns->str_len) {
          insert_into_tree(top_node, line, column, LEFT);
        } else {
          insert_into_tree(top_node, line, column, RIGHT);
        }

      } else if (top_node != NULL && ch == '^') {
        int left_col = column - 1;
        if (!location_found(columns, column, left_col)) {
          // printf("beam splitted left to line:[%d],column:[%d]\n", line,
          //        left_col);
          debug_state[line][left_col] = '|';
          columns->array_ptr[left_col] = '|';
          insert_into_tree(top_node, line, left_col, LEFT);
        }
        int right_col = column + 1;
        if (!location_found(columns, column, right_col)) {
          // printf("beam splitted right to line:[%d],column:[%d]\n", line,
          //        right_col);
          debug_state[line][right_col] = '|';
          columns->array_ptr[right_col] = '|';
          insert_into_tree(top_node, line, right_col, RIGHT);
        }
        times_splitted++;
      }
    }
  }

  print_debug_state(debug_state, result->length, result->lines[0]->str_len);
  print_tree(parent);

  free_tree(parent);
  free_debug_state(debug_state, result->length);

  count = times_splitted;

  return count;
}