/* src/squares.c */
#include "squares.h"
#include "squarify.h"
#include "tree.h"
#include <libgen.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

void draw_treemap(Node *root) {
  initscr();
  cbreak();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);

  int max_y, max_x;
  getmaxyx(stdscr, max_y, max_x);

  attron(A_REVERSE);
  mvprintw(0, 0, "SCANNING COMPLETE - Press any key to exit ");
  attroff(A_REVERSE);
  refresh();

  int limit = (root->child_count < 15) ? root->child_count : 15;
  if (limit == 0) {
    getch();
    endwin();
    return;
  }

  Area *areas = malloc(limit * sizeof(Area));
  Rect *rects = calloc(limit, sizeof(Rect));

  double screen_area = max_x * (max_y - 1);
  double total_bytes = 0;

  for (int i = 0; i < limit; i++) {
    total_bytes += root->children[i]->bytes;
  }
  if (total_bytes <= 0)
    total_bytes = 1;

  for (int i = 0; i < limit; i++) {
    double val = ((double)root->children[i]->bytes / total_bytes) * screen_area;
    if (val < 1.0)
      val = 1.0;
    areas[i].value = val;
    areas[i].index = i;
  }

  Rect bounds = {0, 1, (double)max_x, (double)(max_y - 1), -1};
  squarify(areas, limit, bounds, rects);

  WINDOW **blocks = calloc(limit, sizeof(WINDOW *));

  for (int i = 0; i < limit; i++) {
    Rect r = rects[i];

    int w_h = (int)r.h;
    int w_w = (int)r.w;

    if (w_h < 1)
      w_h = 1;
    if (w_w < 1)
      w_w = 1;

    blocks[i] = newwin(w_h, w_w, (int)r.y, (int)r.x);

    if (blocks[i]) {
      if (w_h >= 3 && w_w >= 3) {
        box(blocks[i], 0, 0);
      }

      Node *child = root->children[r.index];
      char *dir_name = basename(child->path);

      if (w_h >= 2 && w_w >= 4) {
        mvwprintw(blocks[i], 1, 1, "%.*s", w_w - 2, dir_name);
      }
      wrefresh(blocks[i]);
    }
  }

  getch();

  for (int i = 0; i < limit; i++) {
    if (blocks[i])
      delwin(blocks[i]);
  }
  free(blocks);
  free(areas);
  free(rects);
  endwin();
}
