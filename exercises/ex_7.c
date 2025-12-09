#include "ex_7.h"
#include <stdio.h>
#include <stdlib.h>

typedef enum { false, true } bool;

char **alloc_memory_for_debug_state(array_string *result) {
  char **debug_state = malloc(result->length * sizeof(char *));

  for (int i = 0; i < result->length; i++) {
    debug_state[i] = malloc(result->lines[i]->str_len * sizeof(char));
  }

  return debug_state;
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

long ex_7(array_string *result) {

  long count = 0;

  int times_splitted = 0;

  char **debug_state = alloc_memory_for_debug_state(result);

  for (int column = 0; column < result->lines[0]->str_len; column++) {
    debug_state[0][column] = result->lines[0]->array_ptr[column];
    if (result->lines[0]->array_ptr[column] == 'S') {
      result->lines[1]->array_ptr[column] = '|';
      break;
    }
  }

  for (int line = 1; line < result->length; line++) {
    line_string *columns = result->lines[line];

    for (int column = 0; column < columns->str_len; column++) {
      char ch = columns->array_ptr[column];
      // todo:only do the split IF the top level has beam
      debug_state[line][column] = ch;
      line_string *top_line = result->lines[line - 1];
      bool has_top_beam = location_found(top_line, top_line->str_len, column);

      if (has_top_beam && ch != '^') {
        columns->array_ptr[column] = '|';
        debug_state[line][column] = '|';
      } else if (has_top_beam && ch == '^') {
        int left_col = column - 1;
        if (!location_found(columns, column, left_col)) {
          // printf("beam splitted left to line:[%d],column:[%d]\n", line,
          //        left_col);
          debug_state[line][left_col] = '|';
          columns->array_ptr[left_col] = '|';
        }
        int right_col = column + 1;
        if (!location_found(columns, column, right_col)) {
          // printf("beam splitted right to line:[%d],column:[%d]\n", line,
          //        right_col);
          debug_state[line][right_col] = '|';
          columns->array_ptr[right_col] = '|';
        }
        times_splitted++;
      }
    }

    // printf("----------------------------------------\n");
  }

  // debug_beam_locations(beam_locations, beams_allocated);
  //print_debug_state(debug_state, result->length, result->lines[0]->str_len);
  free_debug_state(debug_state, result->length);

  count = times_splitted;

  return count;
}