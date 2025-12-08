#include "utils.h"
#include <stdlib.h>
#include <string.h>

int index_of(char *line, char needle, int start, int len) {
  for (int i = start; i < len; i++) {
    if (line[i] == needle) {
      return i;
    }
  }

  return -1;
}

long long to_long(char *line) { return atoll(line); }

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
