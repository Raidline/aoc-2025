#include "ex_2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct sequence_id {
  long number;
  int number_len;
} sequence_id;

char *split_number(long number, int start, int end, int number_len) {
  int splitted_len = number_len / 2;
  char *str = malloc(splitted_len * (sizeof(char *)));
  sprintf(str, "%li", number);
  char *subbuff = malloc(splitted_len / 2 * (sizeof(char *)));

  strncpy(subbuff, str + start, end + 1);

  return subbuff;
}

// // 11, 22 -> invalid
// long invalid_id_value(sequence_id *id) {
//   long number = id->number;

//   char *str_left =
//       split_number(number, 0, (id->number_len / 2) - 1, id->number_len);

//   char *str_right =
//       split_number(number, id->number_len / 2, id->number_len,
//       id->number_len);

//   int cmp = strcmp(str_left, str_right);

//   if (cmp != 0) {
//     return 0;
//   }

//   return number;
// }

// 11, 22 -> invalid
long invalid_id_value_2(sequence_id *id) {
  long number = id->number;

  char *str = malloc(id->number_len * (sizeof(char *)));
  sprintf(str, "%li", number);

  char base = str[0];

  for (int i = 1; i < id->number_len; i++) {

    char c = str[i];

    if (c != base) { // we have a new letter

    } else { // same letter

    }

    int cmp = strcmp(str_left, str_right);

    if (cmp == 0) {
      return number;
    }
  }

  return 0;
}

sequence_id *grab_id(char *line_text, int currIndexPointer, int idx) {
  int number_len = idx - currIndexPointer;
  char subbuff[number_len + 1];

  strncpy(subbuff, line_text + currIndexPointer, number_len);

  sequence_id *id = malloc(sizeof(sequence_id));

  id->number = strtol(subbuff, NULL, 10);
  id->number_len = number_len;

  return id;
}

long sum_invalid_ids(sequence_id *lower_bound, sequence_id *upper_bound) {

  long sum = 0;

  long lower_number = lower_bound->number;
  long upper_number = upper_bound->number;

  for (long number = lower_number; number <= upper_number; number++) {
    char str[20];

    sprintf(str, "%li", number);

    size_t number_len = strlen(str);

    sequence_id *invalid_id = malloc(sizeof(sequence_id));
    invalid_id->number = number;
    invalid_id->number_len = number_len;

    sum += invalid_id_value_2(invalid_id);

    free(invalid_id);
  }

  return sum;
}

long ex_2(array_string *result) {

  // this is just one line

  line_string *line = result->lines[0];
  long sum = 0;
  // ranges seperated by ,
  // ids seperated by -

  int idPointer = 0;
  sequence_id *firstNumber;
  sequence_id *secondNumber;

  char *line_text = line->array_ptr;
  for (int i = 0; i <= line->str_len; i++) {
    // if the substring len is impar then it is valid, as we cannot split it in
    // the middle therefore we cannot have sequences repeated twice

    char c = line_text[i];

    if (c == '-') { // id case
      firstNumber = grab_id(line_text, idPointer, i);

      idPointer = i + 1;                         // jump the '-'
    } else if (c == ',' || i == line->str_len) { // split case
      secondNumber = grab_id(line_text, idPointer, i);

      idPointer = i + 1; // jump the ','

      long interval_sum = sum_invalid_ids(firstNumber, secondNumber);

      sum += interval_sum;

      free(firstNumber);
      free(secondNumber);

      printf("------------------------------------------------\n");
    }
  }

  return sum; // 826380671 -> too low
}
