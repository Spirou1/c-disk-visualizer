/* src/squarify.c */

#include "squarify.h"
#include <stdlib.h>

double get_shortest_side(Rect bounds) {
  return (bounds.w < bounds.h) ? bounds.w : bounds.h;
}

double worst(Area *row, int row_count, double w) {
  if (row_count == 0 || w == 0)
    return 0.0;
  double r_max = row[0].value, r_min = row[0].value, s = 0.0;
  for (int i = 0; i < row_count; i++) {
    if (row[i].value > r_max)
      r_max = row[i].value;
    if (row[i].value < r_min)
      r_min = row[i].value;
    s += row[i].value;
  }
  double term1 = ((w * w) * r_max) / (s * s);
  double term2 = (s * s) / ((w * w) * r_min);
  return (term1 > term2) ? term1 : term2;
}

void layout_row(Area *row, int row_count, Rect *bounds, Rect *out_rects,
                int *out_idx) {
  if (row_count == 0)
    return;
  double row_area_sum = 0;
  for (int i = 0; i < row_count; i++)
    row_area_sum += row[i].value;

  int is_horizontal = (bounds->w >= bounds->h);
  double thickness =
      is_horizontal ? (row_area_sum / bounds->h) : (row_area_sum / bounds->w);
  double current_x = bounds->x, current_y = bounds->y;

  for (int i = 0; i < row_count; i++) {
    double rect_w, rect_h;
    if (is_horizontal) {
      rect_w = thickness;
      rect_h = row[i].value / thickness;
      out_rects[*out_idx] =
          (Rect){current_x, current_y, rect_w, rect_h, row[i].index};
      current_y += rect_h;
    } else {
      rect_w = row[i].value / thickness;
      rect_h = thickness;
      out_rects[*out_idx] =
          (Rect){current_x, current_y, rect_w, rect_h, row[i].index};
      current_x += rect_w;
    }
    (*out_idx)++;
  }

  if (is_horizontal) {
    bounds->x += thickness;
    bounds->w -= thickness;
  } else {
    bounds->y += thickness;
    bounds->h -= thickness;
  }
}

void squarify_recursive(Area *areas, int total_areas, int current_idx,
                        Area *row, int row_count, Rect bounds, Rect *out_rects,
                        int *out_idx) {
  if (current_idx >= total_areas) {
    if (row_count > 0)
      layout_row(row, row_count, &bounds, out_rects, out_idx);
    return;
  }
  double w = get_shortest_side(bounds);
  Area c = areas[current_idx];

  double current_worst = worst(row, row_count, w);
  row[row_count] = c;
  double new_worst = worst(row, row_count + 1, w);

  if (row_count == 0 || current_worst >= new_worst) {
    squarify_recursive(areas, total_areas, current_idx + 1, row, row_count + 1,
                       bounds, out_rects, out_idx);
  } else {
    layout_row(row, row_count, &bounds, out_rects, out_idx);
    row[0] = c;
    squarify_recursive(areas, total_areas, current_idx + 1, row, 1, bounds,
                       out_rects, out_idx);
  }
}

void squarify(Area *areas, int total_areas, Rect bounds, Rect *out_rects) {
  Area *row = malloc(total_areas * sizeof(Area));
  int out_idx = 0;
  squarify_recursive(areas, total_areas, 0, row, 0, bounds, out_rects,
                     &out_idx);
  free(row);
}
