#include "ex_9.h"
#include "../core/utils.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  long min_y;
  long max_y;
} column_span;

typedef struct coord_point {
  long x;
  long y;
} coord_point;

typedef struct {
  long y1;
  long y2;
} interval;

typedef struct {
  interval *intervals;
  int count;
} interval_list;

typedef struct rectangle {
  coord_point p1;
  coord_point p2;
  long length;
  long height;
  long area;
} rectangle;

void free_scanlines(interval_list *scanlines, int len) {
  for (int i = 0; i < len; i++) {
    free(scanlines[i].intervals);
  }

  free(scanlines);
}

void free_rectanges(rectangle **rectangles, int len) {
  for (int i = 0; i < len; i++) {
    free(rectangles[i]);
  }

  free(rectangles);
}

void debug_points(coord_point points[], int len) {
  for (int i = 0; i < len; i++) {
    printf("point : [x:%li, y:%li]\n", points[i].x, points[i].y);
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

void debug_scanlines(long min_x, long max_x, interval_list *scanlines) {
  // Print scanlines
  for (long x = min_x; x <= max_x; x++) {
    interval_list *sl = &scanlines[x - min_x];
    printf("x=%ld: ", x);
    for (int i = 0; i < sl->count; i++) {
      printf("[%ld,%ld] ", sl->intervals[i].y1, sl->intervals[i].y2);
    }
    printf("\n");
  }
}

void polygon_bounds(coord_point *poly, int n, long *min_x, long *max_x) {
  *min_x = poly[0].x;
  *max_x = poly[0].x;

  for (int i = 1; i < n; i++) {
    if (poly[i].x < *min_x)
      *min_x = poly[i].x;
    if (poly[i].x > *max_x)
      *max_x = poly[i].x;
  }
}

int cmp_interval(const void *a, const void *b) {
  return ((interval *)a)->y1 - ((interval *)b)->y1;
}

void add_interval(interval_list *list, long y1, long y2) {
  list->intervals =
      realloc(list->intervals, sizeof(interval) * (list->count + 1));
  list->intervals[list->count++] = (interval){y1, y2};
}

void add_crossing(interval_list *list, long y) {
  list->intervals =
      realloc(list->intervals, sizeof(interval) * (list->count + 1));
  list->intervals[list->count++] = (interval){y, y};
}

void build_scanlines(coord_point *poly, int n, interval_list *scanlines,
                     long min_x, long max_x) {
  int width = max_x - min_x + 1;

  for (int i = 0; i < width; i++) {
    scanlines[i].intervals = NULL;
    scanlines[i].count = 0;
  }

  for (int i = 0; i < n; i++) {
    coord_point a = poly[i];
    coord_point b = poly[(i + 1) % n];

    // Only vertical edges contribute
    if (a.x != b.x)
      continue;

    long x = a.x;
    if (x < min_x || x > max_x)
      continue;

    long y1 = MIN(a.y, b.y);
    long y2 = MAX(a.y, b.y);

    // Add BOTH endpoints as crossings
    add_crossing(&scanlines[x - min_x], y1);
    add_crossing(&scanlines[x - min_x], y2);
  }
}

void finalize_scanlines(interval_list *scanlines, long min_x, long max_x) {
  int width = max_x - min_x + 1;

  for (int i = 0; i < width; i++) {
    interval_list *sl = &scanlines[i];

    if (sl->count < 2)
      continue;

    qsort(sl->intervals, sl->count, sizeof(interval), cmp_interval);

    interval *inside = NULL;
    int inside_count = 0;

    for (int j = 0; j + 1 < sl->count; j += 2) {
      long y1 = sl->intervals[j].y1;
      long y2 = sl->intervals[j + 1].y1;

      if (y1 < y2) {
        inside = realloc(inside, sizeof(interval) * (inside_count + 1));
        inside[inside_count++] = (interval){y1, y2};
      }
    }

    free(sl->intervals);
    sl->intervals = inside;
    sl->count = inside_count;
  }
}

void fill_scanline_gaps(interval_list *scanlines, long min_x, long max_x) {
  int width = max_x - min_x + 1;

  interval_list *last = NULL;

  for (int i = 0; i < width; i++) {
    if (scanlines[i].count > 0) {
      last = &scanlines[i];
    } else if (last != NULL) {
      // copy previous interior
      scanlines[i].intervals = malloc(sizeof(interval) * last->count);
      scanlines[i].count = last->count;

      for (int j = 0; j < last->count; j++)
        scanlines[i].intervals[j] = last->intervals[j];
    }
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

void build_column_spans(interval_list *scanlines, long min_x, long max_x,
                        column_span *cols) {
  int width = max_x - min_x + 1;

  for (int i = 0; i < width; i++) {
    cols[i].min_y = LONG_MAX;
    cols[i].max_y = LONG_MIN;

    for (int j = 0; j < scanlines[i].count; j++) {
      interval *iv = &scanlines[i].intervals[j];
      cols[i].min_y = MIN(cols[i].min_y, iv->y1);
      cols[i].max_y = MAX(cols[i].max_y, iv->y2);
    }
  }
}

int rectangle_fits_polygon(rectangle *r, interval_list *scanlines, long min_x) {

  for (long x = r->p1.x; x < r->p2.x; x++) {
    interval_list *col = &scanlines[x - min_x];
    int ok = 0;

    for (int i = 0; i < col->count; i++) {
      interval *iv = &col->intervals[i];

      /* boundary-inclusive */
      if (r->p1.y >= iv->y1 && r->p2.y <= iv->y2 + 1) {
        ok = 1;
        break;
      }
    }

    if (!ok)
      return 0;
  }

  return 1;
}

rectangle *rectangle_inside(rectangle **rects, int rect_count,
                            interval_list *scanlines, long min_x) {
  rectangle *best = NULL;
  long best_area = 0;

  for (int i = 0; i < rect_count; i++) {
    if (!rectangle_fits_polygon(rects[i], scanlines, min_x))
      continue;

    if (rects[i]->area > best_area) {
      best = rects[i];
      best_area = rects[i]->area;
    }
  }

  return best;
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

      rectangle *rect = malloc(sizeof(rectangle));
      long x1 = MIN(p1.x, p2.x);
      long x2 = MAX(p1.x, p2.x) + 1; // +1 ONLY here
      long y1 = MIN(p1.y, p2.y);
      long y2 = MAX(p1.y, p2.y) + 1;

      coord_point rectp1 = {x1, y1};
      coord_point rectp2 = {x2, y2};

      rect->p1 = rectp1;
      rect->p2 = rectp2;
      rect->length = x2 - x1;
      rect->height = y2 - y1;
      rect->area = rect->length * rect->height;

      rectangles[points_len++] = rect;

      if (points_len == cap) {
        cap = cap * 2;
        rectangles = realloc(rectangles, cap * sizeof(rectangle *));
      }
    }
  }
  rectangles = realloc(rectangles, points_len * sizeof(rectangle *));

  long min_x, max_x;
  polygon_bounds(points, result->length, &min_x, &max_x);

  int width = max_x - min_x + 1;

  interval_list *scanlines = calloc(width, sizeof(interval_list));
  column_span *cols = malloc(width * sizeof(column_span));

  build_scanlines(points, result->length, scanlines, min_x, max_x);
  finalize_scanlines(scanlines, min_x, max_x);
  fill_scanline_gaps(scanlines, min_x, max_x);

  build_column_spans(scanlines, min_x, max_x, cols);

  debug_scanlines(min_x, max_x, scanlines);

  rectangle *best = rectangle_inside(rectangles, points_len, scanlines, min_x);

  long longest_area = best ? best->area : 0;

  free_rectanges(rectangles, points_len);
  free_scanlines(scanlines, width);
  free(cols);

  return longest_area;
}

long ex_9(array_string *result) { return ex_9_b(result); }