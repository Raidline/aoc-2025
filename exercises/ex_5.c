#include "ex_5.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

#define MAX(x, y) (((x) > (y)) ? (x) : (y))

typedef enum { false, true } bool;

typedef struct range_value {
  long lower;
  long upper;
} range_value;

void debug_range_matrix(range_value **values, int len) {
  for (int i = 0; i < len; i++) {
    if (values[i] == NULL) {
      printf("NULL value\n");
    } else {
      printf("value : [min : %ld]|[max : %ld]\n", values[i]->lower,
             values[i]->upper);
    }
  }
}

void free_array(range_value **values, int len) {
  for (int i = 0; i < len; i++) {
    free(values[i]);
  }

  free(values);
}

long to_long(char *line) { return atoll(line); }

int index_of(char *line, char needle, int len) {
  for (int i = 0; i < len; i++) {
    if (line[i] == needle) {
      return i;
    }
  }

  return -1;
}

void fill_array(range_value *value[], int len) {
  for (int i = 0; i < len; i++) {
    value[i] = NULL;
  }
}

range_value *grab_range(char *line_text, int currIndexPointer, int len) {
  char leftValue[currIndexPointer];
  char rightValue[len - currIndexPointer];

  strncpy(leftValue, line_text, currIndexPointer);
  strncpy(rightValue, line_text + (currIndexPointer + 1),
          len - currIndexPointer);

  range_value *value = malloc(sizeof(range_value));

  value->lower = to_long(leftValue);
  value->upper = to_long(rightValue);

  return value;
}

int compare_ranges(const void *a, const void *b) {
  const range_value *ra = *(range_value *const *)a;
  const range_value *rb = *(range_value *const *)b;

  if (ra->lower < rb->lower) {
    return -1;
  }
  if (ra->lower > rb->lower) {
    return 1;
  }
  return 0;
}

long count_valid_ranges(range_value **ranges, int len) {
  qsort(ranges, len, sizeof(range_value *), compare_ranges);

  // disjoint values
  for (int i = 0; i < len - 1; i++) {
    if (ranges[i + 1]->lower <= ranges[i]->upper) {
      long new_end = MAX(ranges[i]->upper, ranges[i + 1]->upper);

      ranges[i]->upper = ranges[i + 1]->lower - 1;
      ranges[i + 1]->upper = new_end;
    }
  }

  debug_range_matrix(ranges, len);

  long sum = 0;
  for (int i = 0; i < len; i++) {
    sum += (ranges[i]->upper - ranges[i]->lower + 1);
  }

  // 360246169541235 -> too low
  return sum;
}

bool find_in_array(range_value **values, long needle, int len) {
  for (int i = 0; i < len; i++) {
    if (values[i]->lower == needle) {
      return true;
    }

    if (values[i]->upper == needle) {
      return true;
    }

    if (needle > values[i]->lower && needle < values[i]->upper) {
      return true;
    }
  }

  return false;
}

long ex_5(array_string *result) {

  int len = result->length;
  int max_items = 0;
  range_value **ranges = malloc(len * sizeof(range_value *));
  long count = 0;

  fill_array(ranges, len);

  for (int i = 0; i < result->length; i++) {
    line_string *line = result->lines[i];

    // we could just see if the line is equal to " "
    // if is equal searching mode is on
    // if not and not searching mode do the splitting
    // if not and searching, apply the search

    // if (searching) {
    //   bool found = find_in_array(ranges, to_long(line->array_ptr),
    //   max_items);

    //   if (found) {
    //     count++;
    //   }
    // } else {
    //   if (strlen(line->array_ptr) == 0) {
    //     searching = true;
    //     ranges = realloc(ranges, max_items * sizeof(*ranges));
    //     debug_range_matrix(ranges, max_items);
    //     continue;
    //   }

    if (strlen(line->array_ptr) == 0) {
      ranges = realloc(ranges, max_items * sizeof(*ranges));
      // debug_range_matrix(ranges, max_items);
      count = count_valid_ranges(ranges, max_items);
      break;
    }

    int splitIdx = index_of(line->array_ptr, '-', line->str_len);
    if (splitIdx == -1) {
      printf("something very wrong has happened in the code, we should get "
             "here.\n");

      return -1;
    }
    range_value *range = grab_range(line->array_ptr, splitIdx, line->str_len);
    ranges[max_items++] = range;
  }
  //}

  free_array(ranges, len);

  return count;
}