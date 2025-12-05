#include "read_file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

array_string *read_file(const char *filename) {
  FILE *f = fopen(filename, "r");
  if (!f) {
    return NULL;
  }

  line_string **lines = NULL;
  int count = 0;
  int cap = 0;

  char *buf = NULL;
  size_t bufsize = 0;
  ssize_t len;

  array_string *file_data = malloc(sizeof(array_string));

  while ((len = getline(&buf, &bufsize, f)) != -1) {
    if (count == cap) {
      cap = (cap == 0 ? 4 : cap * 2);
      lines = realloc(lines, cap * sizeof(char *));
    }

    line_string *line_data = malloc(sizeof(line_string));
    line_data->array_ptr = malloc(len);
    memcpy(line_data->array_ptr, buf, len);
    line_data->array_ptr[strlen(line_data->array_ptr) - 1] = '\0';

    line_data->str_len = len;

    lines[count++] = line_data;
  }

  free(buf);
  fclose(f);

  file_data->length = count;
  file_data->lines = lines;

  return file_data;
}
