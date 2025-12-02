#include "ex_2.h"
#include "../core/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct sequence_id {
  int number;
  int number_len;
} sequence_id;

char *split_number(int number, int start, int end) {
  int number_len = end - start;
  char *str = malloc(number_len * (sizeof(char *)));
  sprintf(str, "%d", number);
  char subbuff[number_len];
  strncpy(subbuff, str + start, end);

  return str;
}

int invalid_id_value(sequence_id *id) {
  int number = id->number;

  char *str_left = split_number(number, 0, id->number_len / 2);
  char *str_right = split_number(number, id->number_len / 2, id->number_len);

  int cmp = strcmp(str_left, str_right);

  if (cmp != 0) {
    return 0;
  }

  return number;
}

sequence_id *grab_id(char *line_text, int index, int currIdxPointer) {
  int number_len = index - currIdxPointer;
  char subbuff[index - currIdxPointer];

  strncpy(subbuff, line_text + index, number_len);

  sequence_id *id = malloc(sizeof(sequence_id));

  id->number = convert_char_to_number(subbuff, number_len);
  id->number_len = number_len;

  return id;
}

int sum_invalid_ids(sequence_id *lower_bound, sequence_id *upper_bound) {

  // impar values do not count
  if (lower_bound->number_len % 2 == 1 && upper_bound->number_len % 2 == 1) {
    return 0;
  }

  int sum = 0;

  int lower_number = lower_bound->number;
  int upper_number = upper_bound->number;

  sum += invalid_id_value(lower_bound);
  sum += invalid_id_value(upper_bound);

  for (int number = lower_number; number <= upper_number; number++) {

      //todo: create struct for this number and len -> then pass it to the function

    char str[] = {};

    sprintf(str, "%d", number);

    size_t number_len = strlen(str);

    // impar do not count
    if (number_len % 2 == 1) {
      continue;
    }

    int number_len = index - currIdxPointer;
    char subbuff[index - currIdxPointer];

    strncpy(subbuff, line_text + index, number_len);

    // 11, 22 -> invalid
  }

  return sum;
}

int ex_2(array_string *result) {

  // this is just one line

  line_string *line = result->lines[0];
  int sum = 0;
  // ranges seperated by ,
  // ids seperated by -

  int idPointer = 0;
  sequence_id *firstNumber;
  sequence_id *secondNumber;
  for (int i = 0; line->str_len; i++) {
    // if the substring len is impar then it is valid, as we cannot split it in
    // the middle therefore we cannot have sequences repeated twice
    char *line_text = line->array_ptr;

    char c = line_text[i];

    if (c == '-') { // id case
      firstNumber = grab_id(line_text, i, idPointer);

      idPointer = i + 1;   // jump the '-'
    } else if (c == ',') { // split case
      secondNumber = grab_id(line_text, i, idPointer);

      idPointer = i + 1; // jump the ','

      int interval_sum = sum_invalid_ids(firstNumber, secondNumber);

      sum += interval_sum;

      free(firstNumber);
      free(secondNumber);
    }
  }

  return sum;
}
