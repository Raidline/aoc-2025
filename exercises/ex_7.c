#include "ex_7.h"
#include <stdio.h>

typedef enum { false, true } bool;

bool location_found(int beam_locations[], int len, int needle) {
  bool location_found = false;
  for (int c = 0; c < len; c++) {
    if (beam_locations[c] == needle) {
      location_found = true;
      break;
    }
  }

  return location_found;
}

long ex_7(array_string *result) {

  long count = 0;

  int beam_locations[500];
  int beam_location_idx = 0;

  for (int column = 0; column < result->lines[0]->str_len; column++) {
    if (result->lines[0]->array_ptr[column] == 'S') {
      beam_locations[beam_location_idx++] = column;
      break;
    }
  }

  for (int line = 1; line < result->length; line++) {
    line_string *columns = result->lines[line];

    for (int column = 0; column < columns->str_len; column++) {
      char ch = columns->array_ptr[column];
      if (ch == '^') { // split
        int left_col = column - 1;
        if (!location_found(beam_locations, beam_location_idx, left_col)) {
          beam_locations[beam_location_idx++] = left_col;
          printf("beam splitted left to line:[%d],column:[%d]\n", line,
                 left_col);
        }
        int right_col = column + 1;
        if (!location_found(beam_locations, beam_location_idx, right_col)) {
          beam_locations[beam_location_idx++] = right_col;
          printf("beam splitted right to line:[%d],column:[%d]\n", line,
                 right_col);
        }
      }
    }

    printf("----------------------------------------\n");
  }

  count = beam_location_idx - 1;

  return count;
}