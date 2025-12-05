#include "ex_5.h"
#include <stdlib.h>

// order the ranges by the left value
// then find the range
//
//
// 3-5
// 10-14
// 16-20
// 12-18
//
// [3,10,12,16] -> arr
// 3 -> 5; 10 -> 14; 12 -> 18; 16-> 20
//
// Is 1 valid?
//
// if 1 <= arr[0], then false
//
// [3,10,12,16] -> arr
//
// BINARY SEARCH PLAN
//
// Chose mid value as pivot
// if val >= pivot -> see if value is inside pivot bounds
// if NOT get right items -> chose new pivot -> if val >= pivot -> repeat
// process else if val < pivot -> get left items -> chose new pivot -> repeat
// until its >= than pivot
//
// if val == pivot, then check upper bound
// if val < pivot.upper -> VALID
// if val > pivot.upper -> check right side (higher values)
//
// Is 5 valid?
//
// 5 >= arr[0], then binary search
//
//
// pivot -> 10; 5 < 10, check left remaining items
// break into [3,10]
// 5 < 10, break left again -> [3]
//
// 5 > 3 -> see upper bound
// 3 ->5; then 5 <= 5 => true
//
// [3,10,12,16] -> arr
// Is 11 valid?
//
// 11 >= arr[0], then binary search
//
// pivot -> 10; 11 > 10 -> see upper bound
// 10->14; 11 <= 14 => true
//

typedef enum { false, true } bool;

typedef struct range_value {
  long lower;
  long upper;
} range_value;

// binary_search
bool find_in_array(range_value *values[], long value) { return false; }

void insert_into_array(range_value *value, range_value *values[]) {
  // do stuff
}

range_value *create_range(line_string *line, int splitIdx) {
  range_value *value = malloc(sizeof(range_value));

  // split the values

  return value;
}

int ex_5(array_string *result) {

  range_value *ranges[result->length];
  bool searching = false;
  int count = 0;

  for (int i = 0; i < result->length; i++) {
    line_string *line = result->lines[i];

    for (int j = 0; j < line->str_len; j++) {
      char c = line->array_ptr[j];

      if (searching) {
        // get the long value from this line
        // search the value
        //
        int found = find_in_array(ranges, -1);

        if (found) {
          count++;
        }

        continue;
      }

      if (c == '-') {
        // it is a split
        //
        // get left value
        // get right value
        // create struct of both
        // place them inside the array and sort it
        //
        continue; // we alredy have both values no need to continue iterating
                  // this line
      }

      if (c == ' ') {
        // we are in the finding part.
        // start applying binary search.
        searching = true;
      }
    }
  }

  return count;
}