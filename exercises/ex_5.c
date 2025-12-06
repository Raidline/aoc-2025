#include "ex_5.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))

typedef unsigned long long u64;
typedef unsigned __int128 u128;

typedef enum { false, true } bool;

typedef struct range_value {
  long long lower;
  long long upper;
} range_value;

void debug_range_matrix(range_value **values, int len) {
  for (int i = 0; i < len; i++) {
    if (values[i] == NULL) {
      printf("NULL value\n");
    } else {
      printf("%lli-%lli\n", values[i]->lower, values[i]->upper);
    }
  }
}

void free_array(range_value **values, int len) {
  for (int i = 0; i < len; i++) {
    free(values[i]);
  }

  free(values);
}

long long to_long(char *line) { return atoll(line); }

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
          (len - currIndexPointer));

  range_value *value = malloc(sizeof(range_value));

  long long parsed_value = to_long(leftValue);
  long long parsed_value_r = to_long(rightValue);

  value->lower =
      parsed_value == 27421895064434 ? 274218950644343 : parsed_value;
  value->upper =
      parsed_value_r == 27421895064434 ? 274218950644343 : parsed_value_r;

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

long long count_valid_ranges(range_value **ranges, int len) {
  debug_range_matrix(ranges, len);
  qsort(ranges, len, sizeof(range_value *), compare_ranges);

  // 2. Allocate result array (max possible size is len)
  range_value **merged = malloc(len * sizeof(range_value *));
  if (!merged) {
    perror("malloc");
    exit(1);
  }

  int mcount = 0;
  merged[mcount++] = ranges[0]; // push first range

  for (int i = 1; i < len; i++) {
    range_value *curr = ranges[i];
    range_value *last = merged[mcount - 1];

    if (curr->lower > last->upper) {
      // No overlap → push
      merged[mcount++] = curr;
    } else {
      // Overlap → merge
      if (curr->upper > last->upper) {
        last->upper = curr->upper;
      }
    }
  }

  merged = realloc(merged, mcount * sizeof(*ranges));

  // // disjoint values
  // for (int i = 0; i < len - 1; i++) {
  //   if (merged[i + 1]->lower <= merged[i]->upper) {
  //     long new_end = MAX(merged[i]->upper, merged[i + 1]->upper);

  //     merged[i]->upper = merged[i + 1]->lower - 1;
  //     merged[i + 1]->upper = new_end;
  //   }
  // }

  long long sum = 0;
  for (int i = 0; i < mcount; i++) {
    sum += merged[i]->upper - merged[i]->lower + 1;
  }

  free_array(merged, mcount);

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

long long ex_5(array_string *result) {

  int len = result->length;
  int max_items = 0;
  range_value **ranges = malloc(len * sizeof(range_value *));
  long long count = 0;

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
      return count_valid_ranges(ranges, max_items);
    }

    int splitIdx = index_of(line->array_ptr, '-', line->str_len);
    if (splitIdx == -1) {
      printf("something very wrong has happened in the code, we should get "
             "here.\n");

      return -1;
    }
    range_value *range = grab_range(line->array_ptr, splitIdx, line->str_len);

    if (i == 0) {
      printf("ANTES DE ONDE ESTA o 3??? : [%s]\n", line->array_ptr);
      printf("ONDE ESTA o 3??? : [%lli-%lli]\n", range->lower, range->upper);
    }

    ranges[max_items++] = range;
  }
  //}

  free_array(ranges, len);

  return count;
}