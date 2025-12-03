#include "core/read_file.h"
//#include "exercises/ex_1.h"
//#include "exercises/ex_2.h"
#include "exercises/ex_3.h"
#include <stdio.h>
#include <stdlib.h>

void free_array_struct(array_string *pointer) {
  for (int i = 0; i < pointer->length; i++) {
    line_string *line_data = pointer->lines[i];

    free(line_data->array_ptr);
    free(line_data);
  }

  free(pointer->lines);
  free(pointer);
}


int main() {
  array_string *result = read_file("input/ex3_single.txt");

  //printf("%d", ex_1(result));
  //printf("%li", ex_2(result));
  printf("%li", ex_3(result));


  free_array_struct(result);

  return 0;
}
