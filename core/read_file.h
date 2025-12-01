#ifndef READ_FILE_H
#define READ_FILE_H

typedef struct line_string {
  char *array_ptr;
  int str_len;
} line_string;

typedef struct array_string {
  int length;
  struct line_string** lines;
} array_string;

array_string *read_file(const char *filename);

#endif
