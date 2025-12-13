#include "ex_9.h"
#include "../core/utils.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct coord_point {
  long x;
  long y;
} coord_point;

typedef struct boundarie_point {
  long x1;
  long x2;
  long y;
} boundarie_point;

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

void debug_bondaries(boundarie_point boundaries[], int len) {
  for (int i = 0; i < len; i++) {
    boundarie_point p = boundaries[i];
    printf("boundarie of x: >[%li] & <[%li], y:[%li]\n", p.x1, p.x2, p.y);
  }
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

int compare_coords(const void *a, const void *b) {
  const boundarie_point ra = *(boundarie_point const *)a;
  const boundarie_point rb = *(boundarie_point const *)b;

  if (ra.y < rb.y) {
    return -1;
  }
  if (ra.y > rb.y) {
    return 1;
  }
  return 0;
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

int compare_lines(const void *a, const void *b) {
  const coord_point ra = *(coord_point const *)a;
  const coord_point rb = *(coord_point const *)b;

  if (ra.y < rb.y) {
    return -1;
  }
  if (ra.y > rb.y) {
    return 1;
  }
  return 0;
}

long ex_9_a(array_string *result) {
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

  // debug_rectangles(rectangles, points_len);

  long longest_area = rectangles[0]->area;

  free_rectanges(rectangles, points_len);

  return longest_area;
}

long ex_9_b(array_string *result) {
  coord_point points[result->length];
  int cap = result->length;
  rectangle **rectangles = malloc(cap * sizeof(rectangle *));
  int points_len = 0;
  for (int i = 0; i < result->length; i++) {
    line_string *line = result->lines[i];
    coord_point p1 = get_point(line);
    points[i] = p1;

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
  rectangles = realloc(rectangles, points_len * sizeof(rectangle *));
  qsort(points, result->length, sizeof(coord_point), compare_lines);
  // todo: !improve this calculation! - not sure we can do it per line
  //
  // todo: do all the area calculations at the same time, and then filter
  boundarie_point
      boundaries[points[result->length - 1].y]; // boundaries per line

  for (int i = 0; i < points[result->length - 1].y; i++) {
    boundarie_point p = {-1, -1, i};
    boundaries[i] = p;
  }

  long last_line = 0;
  for (int i = 0; i < result->length - 1; i++) {
    coord_point p1 = points[i];
    coord_point p2 = points[i + 1];

    while (p2.y == p1.y) {
      if (last_line != 0) { // do the missing line
        long missing_line = p1.y - last_line;

        boundarie_point curr = boundaries[missing_line];
        // todo: to the calculation for the missing line using the columns
        long min_x = MIN(p1.x, p2.x);
        long calculated_min = curr.x1 == -1 ? min_x : MIN(min_x, curr.x1);
        long max_x = MAX(p1.x, p1.x);
        long calculated_max = curr.x2 == -1 ? max_x : MAX(max_x, curr.x2);
        curr.x1 = calculated_min;
        curr.x2 = calculated_max;
      }
      // adapt the calculation for the line
      boundarie_point curr = boundaries[p1.y];
      long min_x = MIN(p1.x, p2.x);
      long calculated_min = curr.x1 == -1 ? min_x : MIN(min_x, curr.x1);
      long max_x = MAX(p1.x, p1.x);
      long calculated_max = curr.x2 == -1 ? max_x : MAX(max_x, curr.x2);
      curr.x1 = calculated_min;
      curr.x2 = calculated_max;
    }

    last_line = p1.y;
  }

  debug_bondaries(boundaries, result->length);

  qsort(boundaries, result->length, sizeof(coord_point), compare_coords);

  // todo: filter for the boundaries now

  // debug_rectangles(rectangles, points_len);

  qsort(rectangles, points_len, sizeof(rectangle *), compare_areas);
  long longest_area = rectangles[0]->area;

  free_rectanges(rectangles, points_len);

  return longest_area;
}

long ex_9(array_string *result) { return ex_9_b(result); }