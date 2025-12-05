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

typedef enum { false, true } bool;

typedef struct range_value {
  long lower;
  long upper;
} range_value;

void debug_range_matrix(range_value **values, int len) {
  for (int i = 0; i < len; i++) {
    printf("value : [min : %li]|[max : %li]\n", values[i]->lower,
           values[i]->upper);
  }
}

void free_array(range_value **values, int len) {
  for (int i = 0; i < len; i++) {
    free(values[i]);
  }

  free(values);
}

long to_long(char *line) { return strtol(line, NULL, 10); }

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
    range_value *val = malloc(sizeof(range_value));
    val->lower = 2147483647;
    val->upper = 2147483647;

    value[i] = val;
  }
}

range_value *grab_range(char *line_text, int currIndexPointer, int len) {
  char leftValue[currIndexPointer];
  char rightValue[len - currIndexPointer];

  // what about terminator?
  strncpy(leftValue, line_text, currIndexPointer);
  strncpy(rightValue, line_text + (currIndexPointer + 1),
          len - currIndexPointer);

  range_value *value = malloc(sizeof(range_value));

  value->lower = to_long(leftValue);
  value->upper = to_long(rightValue);

  return value;
}

bool find_in_array(range_value **values, long needle, int len) {
  int lo = 0;
  int hi = len;

  do {
    int mid = (lo + (hi - lo) / 2);
    range_value *value = values[mid];

    if (value->lower == needle) {
      if (value->upper < needle) {
        return true;
      }
    } else if (value->lower > needle) {
      hi = mid;
    } else {
      if (value->upper <= needle) {
        return true;
      }
      lo = mid + 1;
    }
  } while (lo < hi);

  return false;
}

int find_lowest_insertion_point_in_array(range_value **values, long needle,
                                         int len) {
  int lo = 0;
  int hi = len;

  do {
    int mid = (lo + (hi - lo) / 2);
    range_value *value = values[mid];

    if (value->lower == needle) {
      if (needle < value->upper) {
        return mid - 1; // go to the left
      }

      return mid;
    } else if (value->lower > needle) {
      hi = mid;
    } else {
      return mid + 1;
    }
  } while (lo < hi);

  return 0;
}

int partition(range_value **values, int lo, int hi) {
  range_value *pivot = values[hi / 2];

  int idx = lo - 1;

  for (int i = lo; i < hi; i++) {
    if (values[i]->lower <= pivot->lower) {
      idx++;
      range_value *tmp = values[i];
      values[i] = values[idx];
      values[idx] = tmp;
    }
  }

  idx++;
  values[hi] = values[idx];
  values[idx] = pivot;

  return idx;
}

// this is putting all the NULLS at the end, so we can "trim" the array
void sort_array(range_value **values, int lo, int hi) {

  if (lo >= hi) {
    return;
  }

  int pivotIdx = partition(values, lo, hi);

  sort_array(values, lo, pivotIdx - 1);
  sort_array(values, pivotIdx + 1, hi);
}

bool insert_into_array(range_value **values, range_value *value, int len) {
  if (len == 0) {
    values[0] = value;
    return true;
  }

  if (len == 1) {
    if ((values[0]->lower == value->lower) &&
        (values[0]->upper == value->upper)) {
      // ignore this entry
      return false;
    }

    if (value->lower < values[0]->lower) {
      values[1] = value;
    } else if (values[0]->lower > value->lower) {
      values[1] = values[0];
      values[0] = value;
    } else {
      if (value->upper < values[0]->upper) {
        values[1] = values[0];
        values[0] = value;
      } else {
        values[1] = value;
      }
    }
  }

  // new logic where we need to find the lowest point, and then go through until
  // we get the highest point (new.higher >= higher)
  int point = find_lowest_insertion_point_in_array(values, value->lower, len);

  printf("lower point of insertion : %d - len : %d\n", point, len);

  if (point == len) {
    values[len] = value;

    return true;
  }

  for (int i = point; i < len; i++) {
    range_value *curr = values[i];

    if (value->upper < curr->upper) {
      for (int j = i; j < len; j++) {
        values[j + 1] = curr;
      }
      values[i] = value;
      return true;
    } else if (value->upper >= curr->upper) {
      values[i] = malloc(sizeof(range_value));
      values[i]->lower = 2147483647;
      values[i]->upper = 2147483647;
    }
  }

  values[point] = value;

  return true;
}

int ex_5(array_string *result) {

  int len = result->length;
  int max_items = 0;
  range_value **ranges = malloc(len * sizeof(range_value *));
  bool searching = false;
  int count = 0;

  fill_array(ranges, len);

  for (int i = 0; i < result->length; i++) {
    line_string *line = result->lines[i];

    // we could just see if the line is equal to " "
    // if is equal searching mode is on
    // if not and not searching mode do the splitting
    // if not and searching, apply the search

    if (searching) {
      bool found = find_in_array(ranges, to_long(line->array_ptr), max_items);

      if (found) {
        count++;
      }
    } else {
      printf("looking at string : [%s|%li]\n", line->array_ptr,
             strlen(line->array_ptr));
      if (strlen(line->array_ptr) == 0) {
        searching = true;
        // todo: !segfault here!
        //  start debugging shit
        printf("before sorting...\n");
        debug_range_matrix(ranges, len);

        // todo: issue is in sorting
        //sort_array(ranges, 0, len - 1);
        printf("before realloc...\n");
        debug_range_matrix(ranges, len);
        ranges = realloc(ranges, max_items * sizeof(*ranges));
        printf("after realloc...\n");
        debug_range_matrix(ranges, len);
        continue;
      }

      int splitIdx = index_of(line->array_ptr, '-', line->str_len);
      if (splitIdx == -1) {
        printf("something very wrong has happened in the code, we should get "
               "here.\n");

        return -1;
      }
      range_value *range = grab_range(line->array_ptr, splitIdx, line->str_len);
      if (insert_into_array(ranges, range, max_items)) {
        max_items++;
      }
    }
  }

  free_array(ranges, len);

  return count;
}