#include "ex_4.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct point {
  int y;
  int x;
} point;

typedef enum {
  UP,
  DOWN,
  LEFT,
  RIGHT,
  UP_LEFT,
  UP_RIGHT,
  DOWN_LEFT,
  DOWN_RIGHT,
  DIRECTIONS_COUNT // last for array size
} Direction;

static const point direction_vectors[DIRECTIONS_COUNT] = {
    [UP] = {-1, 0},        [DOWN] = {1, 0},      [LEFT] = {0, -1},
    [RIGHT] = {0, 1},      [UP_LEFT] = {-1, -1}, [UP_RIGHT] = {-1, 1},
    [DOWN_LEFT] = {1, -1}, [DOWN_RIGHT] = {1, 1}};

void debug_positions_array(point positions[], int len) {
  for (int i = 0; i < len; i++) {
    point p = positions[i];
    printf("[y:%d]-[x:%d]\n", p.y, p.x);
  }

  printf("-------------------------------------------\n");
}

void debug_matrix(array_string *values) {

  for (int i = 0; i < values->length; i++) {
    line_string *line = values->lines[i];

    for (int j = 0; j < line->str_len; j++) {
      printf("%c", line->array_ptr[j]);
    }

    printf("\n");
  }

  printf("-------------------------------------------\n");
}

void fill_array(point positions[], int len) {
  for (int i = 0; i < len; i++) {
    point p = {-1, -1};
    positions[i] = p;
  }
}

void remove_positions(array_string *values, point positions[], int arr_len) {
  for (int i = 0; i < arr_len; i++) {
    point p = positions[i];

    if (p.x == -1 || p.y == -1) {
      continue;
    }

    line_string **matrix = values->lines;

    matrix[p.y]->array_ptr[p.x] = 'x';
  }
}

void clear_positions_array(point positions[], int arr_len) {

  for (int i = 0; i < arr_len; i++) {
    point p = {-1, -1};
    positions[0] = p;
  }
}

int count_down_adjacent_points(int x, int max_x_len, array_string *values) {

  int valid_count = 0;

  int valid_dirs_len = x == 0 ? 3 : 5;

  Direction valid_dirs[valid_dirs_len];
  valid_dirs[0] = DOWN;
  if (x == 0 || x == max_x_len - 1) {
    valid_dirs_len = 3;

    if (x == max_x_len - 1) {
      valid_dirs[1] = DOWN_LEFT;
      valid_dirs[2] = LEFT;
    } else {
      valid_dirs[1] = DOWN_RIGHT;
      valid_dirs[2] = RIGHT;
    }
  } else {
    valid_dirs_len = 5;

    valid_dirs[1] = LEFT;
    valid_dirs[2] = DOWN_LEFT;
    valid_dirs[3] = RIGHT;
    valid_dirs[4] = DOWN_RIGHT;
  }

  line_string **matrix = values->lines;

  for (int i = 0; i < valid_dirs_len; i++) {
    point p = direction_vectors[valid_dirs[i]];
    if (matrix[p.y]->array_ptr[x + p.x] == '@') {
      valid_count++;
    }
  }

  return valid_count;
}
int count_up_adjacent_points(int max_len, int x, int max_x_len,
                             array_string *values) {
  int y = max_len - 1;
  int valid_count = 0;

  int valid_dirs_len = x == 0 ? 3 : 5;

  Direction valid_dirs[valid_dirs_len];
  valid_dirs[0] = UP;
  if (x == 0 || x == max_x_len - 1) {
    valid_dirs_len = 3;

    if (x == max_x_len - 1) {
      valid_dirs[1] = UP_LEFT;
      valid_dirs[2] = LEFT;
    } else {
      valid_dirs[1] = UP_RIGHT;
      valid_dirs[2] = RIGHT;
    }
  } else {
    valid_dirs_len = 5;

    valid_dirs[1] = LEFT;
    valid_dirs[2] = UP_LEFT;
    valid_dirs[3] = RIGHT;
    valid_dirs[4] = UP_RIGHT;
  }

  line_string **matrix = values->lines;

  for (int i = 0; i < valid_dirs_len; i++) {
    point p = direction_vectors[valid_dirs[i]];
    if (matrix[y + p.y]->array_ptr[x + p.x] == '@') {
      valid_count++;
    }
  }

  return valid_count;
}
int count_adjacent_points(int y, int x, array_string *values) {
  int valid_count = 0;
  line_string **matrix = values->lines;

  for (int i = 0; i < DIRECTIONS_COUNT; i++) {
    point p = direction_vectors[i];
    if (matrix[y + p.y]->array_ptr[x + p.x] == '@') {
      valid_count++;
    }
  }

  return valid_count;
}

int ex_4(array_string *result) {

  int count = 0;
  int sum = 0;
  int last_line = result->length - 1;
  int arr_len = result->length * result->lines[0]->str_len;
  int positions_idx = 0;
  point positions_to_remove[arr_len];

  fill_array(positions_to_remove, arr_len);
  //debug_positions_array(positions_to_remove, arr_len);

  do {
    count = 0;

    for (int y = 0; y < result->length; y++) {
      line_string *line = result->lines[y];

      for (int x = 0; x < line->str_len; x++) {

        if (line->array_ptr[x] != '@') {
          continue;
        }

        int adjecent_points = 0;

        // printf("looking at point y:[%d], x:[%d]\n", y, x);

        if (y == 0) {
          adjecent_points =
              count_down_adjacent_points(x, line->str_len, result);
        } else if (y == last_line) {
          adjecent_points = count_up_adjacent_points(result->length, x,
                                                     line->str_len, result);
        } else {
          adjecent_points = count_adjacent_points(y, x, result);
        }

        if (adjecent_points < 4) {
          // printf("at point [y:%d-x:%d], found [%d] adjecent points\n", y,
          // x,adjecent_points);

          point to_remove = {y, x};
          positions_to_remove[positions_idx++] = to_remove;

          count++;
        }
      }
    }

    remove_positions(result, positions_to_remove, arr_len);
    clear_positions_array(positions_to_remove, arr_len);

    //debug_matrix(result);

    sum += count;

    printf("removed [%d] rolls\n", count);
    printf("curr sum [%d] rolls\n", sum);
  } while (count > 0);

  return sum;
}