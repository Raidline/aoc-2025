#include "ex_7.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { false, true } bool;

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

  if (node->left != NULL) {
    printf("left -> line:[%d],col:[%d]\n", node->left->line, node->left->col);
  }

  if (node->right != NULL) {
    printf("right -> line:[%d],col:[%d]\n", node->right->line,
           node->right->col);
  }

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

int walk(node *curr) {
  if (curr == NULL) {
    return 0;
  }

  if (curr->left == NULL && curr->right == NULL) {
    return 1;
  }

  return walk(curr->left) + walk(curr->right);
}

int count_tree_paths(node *root) {
  int res = walk(root);
  // if (root == NULL) {
  //   return 1;
  // }

  // printf("line:[%d],col:[%d]\n", root->line, root->col);

  // return count_tree_paths(root->left) + count_tree_paths(root->right);

  return res;
}

bool find_node(node *node, int line, int col) {
  if (node == NULL) {
    return false;
  }

  // printf("needle:[%d],col:[%d]-", line, col);
  // printf("looking into node: line:[%d],col:[%d]\n", node->line, node->col);

  if (node->line == line && node->col == col) {
    return true;
  }

  if (col < node->col) {
    return find_node(node->left, line, col);
  } else {
    return find_node(node->right, line, col);
  }
}

node *create_node(int line, int col) {
  node *n = malloc(sizeof(node));
  n->line = line;
  n->col = col;
  n->left = NULL;
  n->right = NULL;
  return n;
}

node *insert_into_tree(node *root, int line, int col) {
  if (root == NULL) {
    return create_node(line, col);
  }

  //todo: this is what is wrong
  if (root->line == line - 1 && col < root->col) {
    root->left = insert_into_tree(root->left, line, col);
  } else if (root->line == line - 1 && col >= root->col) {
    root->right = insert_into_tree(root->right, line, col);
  }

  return root;
}

long ex_7(array_string *result) {

  long count = 0;

  // int times_splitted = 0;

  char **debug_state = alloc_memory_for_debug_state(result);
  node *parent;

  for (int column = 0; column < result->lines[0]->str_len; column++) {
    debug_state[0][column] = result->lines[0]->array_ptr[column];
    if (result->lines[0]->array_ptr[column] == 'S') {
      result->lines[1]->array_ptr[column] = '|';
      parent = create_node(1, column);
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
      bool node_found = find_node(parent, line - 1, column);

      if (node_found && ch != '^') {
        columns->array_ptr[column] = '|';
        debug_state[line][column] = '|';
        insert_into_tree(parent, line, column);
      } else if (node_found && ch == '^') {
        int left_col = column - 1;
        // if (find_node(parent, line, left_col) == NULL) {
        //  printf("beam splitted left to line:[%d],column:[%d]\n", line,
        //         left_col);
        debug_state[line][left_col] = '|';
        columns->array_ptr[left_col] = '|';
        insert_into_tree(parent, line, left_col);
        // insert_into_tree(parent, line, left_col);
        //}
        int right_col = column + 1;
        // if (find_node(parent, line, right_col) == NULL) {
        //  printf("beam splitted right to line:[%d],column:[%d]\n", line,
        //         right_col);
        debug_state[line][right_col] = '|';
        columns->array_ptr[right_col] = '|';
        insert_into_tree(parent, line, right_col);
        //}
        // times_splitted++;
      }
    }
  }

  print_debug_state(debug_state, result->length, result->lines[0]->str_len);
  print_tree(parent);

  printf("paths count: %d\n", count_tree_paths(parent));

  free_tree(parent);
  free_debug_state(debug_state, result->length);

  // count = times_splitted;

  return count;
}