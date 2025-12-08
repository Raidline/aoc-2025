#include "ex_6.h"
#include "../core/utils.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))

typedef struct numbers_holder {
  long numbers[5000];
  int size;
} numbers_holder;

typedef enum { false, true } bool;

typedef enum { MULT, SUM } OPERATION;

void free_struct(numbers_holder **problems, int len) {
  for (int i = 0; i < len; i++) {
    numbers_holder *problem = problems[i];
    free(problem);
  }

  free(problems);
}

void alloc_all_problems(numbers_holder **problems, int len) {
  for (int i = 0; i <= len; i++) {
    problems[i] = malloc(sizeof(numbers_holder));
  }
}

void debug_problems(numbers_holder **problems, int len) {
  for (int i = 0; i < len; i++) {
    printf("number in index:[%d]\n", i);

    for (int j = 0; j < problems[i]->size; j++) {
      printf("number ->%li\n", problems[i]->numbers[j]);
    }
  }
}

long apply_operation_to_numbers(numbers_holder *numbers, OPERATION oper) {

  printf("size of numbers to list : [%d]\n", numbers->size);
  long result = 0;
  for (int i = 0; i < numbers->size; i++) {
    printf("outer number : [%li], oper : [%d]\n", numbers->numbers[i], oper);
    switch (oper) {
    case MULT:
      if (result == 0) {
        result = numbers->numbers[i];
      } else {
        result *= numbers->numbers[i];
      }
      break;
    case SUM:
      result += numbers->numbers[i];
      break;
    }
  }

  return result;
}

long ex_6(array_string *result) {
  long sum = 0;
  int problems_len = 5000;

  numbers_holder **problems = malloc(problems_len * sizeof(numbers_holder *));
  int numbers_len = 0;
  int inner_numbers_index = 0;

  alloc_all_problems(problems, problems_len);
  for (int i = 0; i < result->length - 1; i++) {
    line_string *line = result->lines[i];
    int outer_index = 0;
    int last_number_idx = 0;
    bool is_number = false;

    for (int j = 0; j < line->str_len; j++) {

      //printf("looking at char : [%c]\n", line->array_ptr[j]);

      if (isdigit(line->array_ptr[j])) {
        if (!is_number) {
          is_number = true;
        }
      } else {
        // whitespace
        if (is_number) {
          int len = j - last_number_idx;
          char number[len];

          // printf("indexes -> j: [%d],len:[%d], "
          //        "last_number_idx:[%d]\n",
          //        j, len, last_number_idx);
          strncpy(number, line->array_ptr + last_number_idx, len);

          long value = to_long(number);

          printf("number as a long : [%li]\n", value);
          //  printf("inserting into : [%d|%d]\n", outer_index,
          //         inner_numbers_index);
          problems[outer_index]->numbers[inner_numbers_index] = value;
          problems[outer_index]->size = problems[outer_index]->size + 1;
          outer_index++;

          is_number = false;
          last_number_idx = j + 1;
        }
      }
      printf("---------------------------\n");

      numbers_len = MAX(numbers_len, outer_index);
    }
    inner_numbers_index++;
  }

  debug_problems(problems, numbers_len);

  line_string *last_line = result->lines[result->length - 1];

  printf("last line : [%s]\n", last_line->array_ptr);

  int operation_index = 0;
  for (int i = 0; i < last_line->str_len; i++) {
    if (last_line->array_ptr[i] == '*') {
      sum += apply_operation_to_numbers(problems[operation_index], MULT);
      operation_index++;
    } else if (last_line->array_ptr[i] == '+') {
      sum += apply_operation_to_numbers(problems[operation_index], SUM);
      operation_index++;
    }
  }

  free_struct(problems, numbers_len);

  return sum;
}