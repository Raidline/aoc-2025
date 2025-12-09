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
    printf("index:[%d]->", i);

    for (int j = 0; j < problems[i]->size; j++) {
      printf("%li,", problems[i]->numbers[j]);
    }

    printf("\n");
  }
}

long apply_operation_to_numbers(numbers_holder *numbers, OPERATION oper) {

  // printf("size of numbers to list : [%d]\n", numbers->size);
  printf("oper : [%d]-", oper);
  long result = 0;
  for (int i = 0; i < numbers->size; i++) {
    printf("outer number : [%li],", numbers->numbers[i]);
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

  printf("\n");

  return result;
}

long ex_6(array_string *result) {
  long sum = 0;
  int problems_len = 5000;

  numbers_holder **problems = malloc(problems_len * sizeof(numbers_holder *));
  int inner_numbers_index = 0;

  alloc_all_problems(problems, problems_len);
  // 6.a
  //  for (int i = 0; i < result->length - 1; i++) {
  //    line_string *line = result->lines[i];
  //    int outer_index = 0;
  //    int last_number_idx = 0;
  //    bool is_number = false;

  //   for (int j = 0; j < line->str_len; j++) {

  //     //printf("looking at char : [%c]\n", line->array_ptr[j]);

  //     if (isdigit(line->array_ptr[j])) {
  //       if (!is_number) {
  //         is_number = true;
  //       }
  //     } else {
  //       // whitespace
  //       if (is_number) {
  //         int len = j - last_number_idx;
  //         char number[len];

  //         // printf("indexes -> j: [%d],len:[%d], "
  //         //        "last_number_idx:[%d]\n",
  //         //        j, len, last_number_idx);
  //         strncpy(number, line->array_ptr + last_number_idx, len);

  //         long value = to_long(number);

  //         printf("number as a long : [%li]\n", value);
  //         //  printf("inserting into : [%d|%d]\n", outer_index,
  //         //         inner_numbers_index);
  //         problems[outer_index]->numbers[inner_numbers_index] = value;
  //         problems[outer_index]->size = problems[outer_index]->size + 1;
  //         outer_index++;

  //         is_number = false;
  //         last_number_idx = j + 1;
  //       }
  //     }
  //     printf("---------------------------\n");

  //     numbers_len = MAX(numbers_len, outer_index);
  //   }
  //   inner_numbers_index++;
  // }

  // 6.b
  //
  //

  int number_aux = 0;
  int i = 0;
  line_string *line = result->lines[i];
  int outer_index = 0;
  bool is_problem_break = true;

  int column_idx = line->str_len - 2;

  do {
    char *column = malloc(result->length - 1 * (sizeof(char)));
    // printf("looking at outer index:[%d], and column_idx:[%d]\n", i,
    // column_idx);
    //  printf("looking at char : [%c]\n", line->array_ptr[column_idx]);

    while (i < result->length - 1) {
      // printf("looking at char : [%c]\n", line->array_ptr[column_idx]);
      if (isdigit(line->array_ptr[column_idx])) {
        column[number_aux++] = line->array_ptr[column_idx];
        is_problem_break = false;
      }

      i++;
      line = result->lines[i];
    }

    if (is_problem_break) {
      outer_index++;
      inner_numbers_index = 0;
      i = 0;
      number_aux = 0;
      free(column);
      line = result->lines[i];
    } else {
      is_problem_break = true;
      column[number_aux] = '\0';

      // printf("column : [%s]\n", column);

      long value = to_long(column);

      // printf("number as a long : [%li]\n", value);
      // printf("inserting into : [%d|%d]\n", outer_index, inner_numbers_index);
      problems[outer_index]->numbers[inner_numbers_index++] = value;
      problems[outer_index]->size = problems[outer_index]->size + 1;

      free(column);
      i = 0;
      number_aux = 0;
      line = result->lines[i];
    }
    column_idx--;
    // printf("---------------------------\n");
  } while (column_idx >= 0);

  // printf("outer index final value : [%d]\n", outer_index);

  //debug_problems(problems, outer_index + 1);

  line_string *last_line = result->lines[result->length - 1];

  // printf("last line : [%s]\n", last_line->array_ptr);

  int operation_index = 0;
  for (int i = last_line->str_len - 1; i >= 0; i--) {
    if (last_line->array_ptr[i] == '*') {
      sum += apply_operation_to_numbers(problems[operation_index], MULT);
      operation_index++;
    } else if (last_line->array_ptr[i] == '+') {
      sum += apply_operation_to_numbers(problems[operation_index], SUM);
      operation_index++;
    }
  }

  free_struct(problems, outer_index);

  return sum; // 11229005961234641 -> too high
}