#ifndef SQUARIFY_H
#define SQUARIFY_H

#include "tree.h"

typedef struct {
  double value;
  int index;
} Area;

void squarify(Area *areas, int total_areas, Rect bounds, Rect *out_rects);

#endif
