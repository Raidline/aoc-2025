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

typedef struct point_counter {
  int line;
  int col;
  long counter;
  char c;
} point_counter;

point_counter **alloc_memory_for_debug_state(array_string *result) {
  point_counter **debug_state =
      malloc(result->length * sizeof(point_counter *));

  for (int i = 0; i < result->length; i++) {
    point_counter *p =
        malloc(result->lines[i]->str_len * sizeof(point_counter));
    p->counter = 0;
    debug_state[i] = p;
  }

  return debug_state;
}

void print_debug_state(point_counter **state, int len, int col_len) {
  for (int i = 0; i < len; i++) {
    for (int j = 0; j < col_len; j++) {
      if (state[i][j].counter >= 1) {
        printf("%li", state[i][j].counter);
      } else {
        printf("%c", state[i][j].c);
      }
    }
    printf("\n");
  }
}

void free_debug_state(point_counter **state, int len) {
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

long ex_7(array_string *result) {

  point_counter **debug_state = alloc_memory_for_debug_state(result);

  for (int column = 0; column < result->lines[0]->str_len; column++) {
    point_counter p = {1, column, debug_state[0][column].counter,
                       result->lines[0]->array_ptr[column]};
    debug_state[0][column] = p;
    if (result->lines[0]->array_ptr[column] == 'S') {
      result->lines[1]->array_ptr[column] = '|';
      point_counter p = {1, column, debug_state[1][column].counter + 1, '|'};
      debug_state[1][column] = p;
      break;
    }
  }

  for (int line = 1; line < result->length; line++) {
    line_string *columns = result->lines[line];

    for (int column = 0; column < columns->str_len; column++) {
      char ch = columns->array_ptr[column];
      point_counter p = {line, column, debug_state[line][column].counter, ch};
      debug_state[line][column] = p;
      line_string *top_line = result->lines[line - 1];

      bool has_top_beam = location_found(top_line, top_line->str_len, column);
      if (has_top_beam && ch != '^') {
        columns->array_ptr[column] = '|';
        point_counter p = {line, column,
                           debug_state[line - 1][column].counter +
                               debug_state[line][column].counter,
                           ch};

        debug_state[line][column] = p;
        //  printf("inserting into right:[%d-%d] of:[%d,%d]\n", line, column,
        //         node->line, node->col);
      } else if (has_top_beam && ch == '^') {
        int left_col = column - 1;

        bool left_path = location_found(columns, column, left_col);

        if (left_path) {
          point_counter p = {line, column,
                             debug_state[line - 1][column].counter +
                                 debug_state[line][left_col].counter,
                             ch};

          debug_state[line][left_col] = p;
        } else {
          point_counter p = {line, column,
                             debug_state[line - 1][column].counter, ch};
          debug_state[line][left_col] = p;
        }

        int right_col = column + 1;
        bool right_path = location_found(columns, column, right_col);

        if (right_path) {
          point_counter pr = {line, column,
                              debug_state[line - 1][column].counter +
                                  debug_state[line][right_col].counter,
                              ch};
          debug_state[line][right_col] = pr;
        } else {
          point_counter pr = {line, column,
                              debug_state[line - 1][column].counter, ch};
          debug_state[line][right_col] = pr;
        }

        columns->array_ptr[left_col] = '|';
        columns->array_ptr[right_col] = '|';
        //times_splitted++;
      }
    }
  }

  // print_debug_state(debug_state, result->length, result->lines[0]->str_len);

  point_counter *last_line = debug_state[result->length - 1];

  long found_locations = 0;
  for (int i = 0; i < result->lines[0]->str_len; i++) {
    // printf("last line values:[%li]\n", last_line[i].counter);
    found_locations += last_line[i].counter;
  }

  free_debug_state(debug_state, result->length);

  return found_locations;
}