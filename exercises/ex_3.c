#include "ex_3.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void fill_batts(char voltages[], int max) {

  for (int i = 0; i < max; i++) {
    voltages[i] = '0';
  }

  voltages[max] = '\0';
}

// void swap(char voltages[2], char new) {

//   if (voltages[0] == '0' && voltages[1] == '0') {
//     voltages[0] = new;
//     return;
//   }

//   if (voltages[1] < new) {
//     if (voltages[0] < voltages[1]) {
//       voltages[0] = voltages[1];
//       voltages[1] = new;
//     } else {
//       voltages[1] = new;
//     }
//   } else if (voltages[0] < new) {
//     voltages[0] = new;
//   } else { // see if we the combine is bigger (89 < 92, e.g) -> even if 2 is
//   not
//     // bigger than any
//     char next_comb[3];
//     next_comb[0] = voltages[1];
//     next_comb[1] = new;
//     next_comb[2] = '\0';

//     long curr = strtol(voltages, NULL, 10);
//     long next = strtol(next_comb, NULL, 10);

//     if (next > curr) {
//       voltages[0] = voltages[1];
//       voltages[1] = new;
//     }
//   }
// }

void shift_array(char voltages[], int curr_index, int max) {

  for (int i = curr_index; i < max - 1; i++) {
    voltages[i] = voltages[i + 1];
  }
}

void swap_2(char voltages[], char new, int inserted, int max) {

  if (inserted < max) {
    voltages[inserted] = new;
    return;
  }

  // 811111111111|119
  // 8 -> 8
  // 1 -> 81
  // 1 -> 811
  // 1 -> 8111
  // 1 -> 81111
  // 1 -> 811111
  // 1 -> 8111111
  // 1 -> 81111111
  // 1 -> 811111111
  // 1 -> 8111111111
  // 1 -> 81111111111
  // 1 -> 811111111111
  // continue example tomorrow!

  for (int i = 0; i < max; i++) {
    if (voltages[i] < voltages[i + 1]) {
      shift_array(voltages, i, max);
      voltages[max - 1] = new;
    }
  }
}

long ex_3(array_string *result) {

  long sum = 0;
  int max_batt = 12;

  for (int i = 0; i < result->length; i++) {
    line_string *line = result->lines[i];

    char voltages[max_batt];

    fill_batts(voltages, max_batt);

    for (int j = 0; j < line->str_len; j++) {
      char c = line->array_ptr[j];

      // printf("array state, [%s]\n", voltages);

      swap_2(voltages, c, j, max_batt);

      // printf("array after state, [%s]\n", voltages);
    }

    long num = strtol(voltages, NULL, 10);

    printf("num per line [%li]\n", num);

    sum += num;
  }

  return sum;
}