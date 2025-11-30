#include "read_file.h"
#include <stdio.h>

int main() {
  array_string *result = read_file("test.txt");
  printf("Hello World!");
  return 0;
}
