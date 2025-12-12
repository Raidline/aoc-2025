#include "ex_9.h"
#include "../core/utils.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct coord_point {
  long x;
  long y;
} coord_point;

typedef struct rectangle {
  coord_point p1;
  coord_point p2;
  long length;
  long height;
  long area;
} rectangle;

void free_rectanges(rectangle **rectangles, int len) {
  for (int i = 0; i < len; i++) {
    free(rectangles[i]);
  }

  free(rectangles);
}

void debug_rectangles(rectangle **rectangles, int len) {
  for (int i = 0; i < len; i++) {
    rectangle *rect = rectangles[i];
    printf("rectangle : for points : ([%li,%li];[%li,%li]), with length:[%li] "
           "height:[%li] and area: [%li]\n",
           rect->p1.x, rect->p1.y, rect->p2.x, rect->p2.y, rect->length,
           rect->height, rect->area);
  }
}

coord_point get_point(line_string *line) {
  long x_value = get_value(line->array_ptr, line->str_len, 0);
  int split_idx = index_of(line->array_ptr, ',', 0, line->str_len);
  long y_value = get_value(line->array_ptr, line->str_len, split_idx + 1);

  coord_point p = {x_value, y_value};

  return p;
}

int compare_areas(const void *a, const void *b) {
  const rectangle *ra = *(rectangle *const *)a;
  const rectangle *rb = *(rectangle *const *)b;

  if (ra->area < rb->area) {
    return 1;
  }
  if (ra->area > rb->area) {
    return -1;
  }
  return 0;
}

long ex_9(array_string *result) {

  int cap = result->length;
  rectangle **rectangles = malloc(cap * sizeof(rectangle *));
  int points_len = 0;
  for (int i = 0; i < result->length - 1; i++) {
    line_string *line = result->lines[i];

    coord_point p1 = get_point(line);

    for (int j = i + 1; j < result->length; j++) {
      line_string *next_line = result->lines[j];

      coord_point p2 = get_point(next_line);

      // always sum 1 for the diff as the self also counts;
      long rect_length = labs(p1.x - p2.x) + 1;
      long rect_height = labs(p1.y - p2.y) + 1;

      rectangle *rect = malloc(sizeof(rectangle));
      rect->p1 = p1;
      rect->p2 = p2;
      rect->height = rect_height;
      rect->length = rect_length;
      rect->area = rect_height * rect_length;

      rectangles[points_len++] = rect;

      if (points_len == cap) {
        cap = cap * 2;
        rectangles = realloc(rectangles, cap * sizeof(rectangle *));
      }
    }
  }

  qsort(rectangles, points_len, sizeof(rectangle *), compare_areas);
  rectangles = realloc(rectangles, points_len * sizeof(rectangle *));

  //debug_rectangles(rectangles, points_len);

  long longest_area = rectangles[0]->area;

  free_rectanges(rectangles, points_len);

  return longest_area;
}