#include "ex_1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int convert_char_to_number(char *line, int number_len) {

  char subbuff[number_len];
  strncpy(subbuff, line + 1, number_len);
  subbuff[number_len] = '\0';

  int number = 0;
  int multiplier = 1;

  for (int i = 0; i < number_len - 1; i++, multiplier *= 10) {
    number *= 10;
    int curr = (subbuff[i] - '0');

    number += curr;
  }

  return number;
}

int execute_operation(char dir, int number, int current_rotation) {

  int new_rotation = current_rotation;
  if (dir == 'R') {
    new_rotation = current_rotation + number;
  } else if (dir == 'L') {
    new_rotation = current_rotation - number;
  }

  if (new_rotation < 0) {
    new_rotation += 100;
  }

  int sum = new_rotation % 100;

  return sum;
}

// the number of times the dial is left pointing at 0 after any rotation in the
// sequence.
int ex_1(array_string *result) {

  int current_rotation = 50;
  int dial_point_0_counter = 0;

  for (int i = 0; i < result->length; i++) {
    printf("old rotation: %d\n", current_rotation);
    line_string *line_data = result->lines[i];

    char *line = line_data->array_ptr;

    char dir = line[0];
    int number_len = line_data->str_len - 1;
    int number = convert_char_to_number(line, number_len);

    printf("rotation: %d\n", number);

    current_rotation = execute_operation(dir, number, current_rotation);

    printf("number of rotation that hit 0: %d\n", current_rotation);

    if (current_rotation == 0) {
      dial_point_0_counter++;
    }

    printf("----------------------------------------\n");
  }

  return dial_point_0_counter;
}
