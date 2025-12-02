#include "ex_1.h"
#include "../core/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// int execute_operation(char dir, int number, int current_rotation) {

//   int new_rotation = current_rotation;
//   if (dir == 'R') {
//     new_rotation = current_rotation + number;
//   } else if (dir == 'L') {
//     new_rotation = current_rotation - number;
//   }

//   if (new_rotation < 0) {
//     new_rotation += 100;
//   }

//   int sum = new_rotation % 100;

//   return sum;
// }

int number_zero_clicks(char dir, int number, int current_rotation,
                       int *zero_clicks) {

  int rotation_for_sum = current_rotation;
  int new_rotation = 0;
  if (dir == 'R') {
    new_rotation = rotation_for_sum + number;
  } else if (dir == 'L') {
    new_rotation = rotation_for_sum - number;
  }

  int total_rotations = abs(new_rotation / 100);

  if (new_rotation <= 0 && current_rotation != 0) {
    total_rotations++;
  }

  new_rotation = new_rotation % 100;

  if (new_rotation <= 0) {
    new_rotation += 100;
  }

  *zero_clicks = total_rotations;

  return new_rotation % 100;
}

// the number of times the dial is left pointing at 0 after any rotation in
// the sequence.
int ex_1(array_string *result) {

  int current_rotation = 50;
  int dial_point_0_counter = 0;

  for (int i = 0; i < result->length; i++) {
    line_string *line_data = result->lines[i];

    char *line = line_data->array_ptr;

    char dir = line[0];
    int number_len = line_data->str_len - 1;
    int number = convert_char_to_number(line, number_len);

    // current_rotation = execute_operation(dir, number, current_rotation);
    int zero_clicks = 0;
    current_rotation =
        number_zero_clicks(dir, number, current_rotation, &zero_clicks);

    // if (current_rotation == 0) {
    //   dial_point_0_counter++;
    // }
    //
    dial_point_0_counter += zero_clicks;
  }

  return dial_point_0_counter;
} // 2665 - has to be higher
