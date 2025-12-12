#ifndef UTILS_H
#define UTILS_H

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

int index_of(char *line, char needle, int start, int len);
long long to_long(char *line);
int convert_char_to_number(char *line, int number_len);

#endif
