/* src/squares.c */
#include "squares.h"
#include "squarify.h"
#include "tree.h"
#include <libgen.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DEPTH 2

void draw_node(Node *node, Rect bounds, int depth) {
  if (bounds.w < 4 || bounds.h < 3)
    return;

  WINDOW *win =
      newwin((int)bounds.h, (int)bounds.w, (int)bounds.y, (int)bounds.x);
  if (!win)
    return;
  box(win, 0, 0);

  char *dir_name = basename(node->path);
  mvwprintw(win, 1, 1, "%.*s", (int)bounds.w - 2, dir_name);
  wrefresh(win);
  delwin(win);

  if (depth >= MAX_DEPTH || node->child_count == 0)
    return;

  Rect inner_bounds = {bounds.x + 1, bounds.y + 2, bounds.w - 2, bounds.h - 3};

  if (inner_bounds.w < 2 || inner_bounds.h < 2)
    return;

  int limit = (node->child_count < 12) ? node->child_count : 12;
  Area *areas = malloc(limit * sizeof(Area));
  Rect *rects = calloc(limit, sizeof(Rect));

  double screen_area = inner_bounds.w * inner_bounds.h;
  double total_bytes = 0;
  for (int i = 0; i < limit; i++)
    total_bytes += node->children[i]->bytes;
  if (total_bytes <= 0)
    total_bytes = 1;

  for (int i = 0; i < limit; i++) {
    double val = ((double)node->children[i]->bytes / total_bytes) * screen_area;
    areas[i].value = (val < 1.0) ? 1.0 : val;
    areas[i].index = i;
  }

  squarify(areas, limit, inner_bounds, rects);

  for (int i = 0; i < limit; i++) {
    draw_node(node->children[rects[i].index], rects[i], depth + 1);
  }

  free(areas);
  free(rects);
}

void draw_treemap(Node *root) {
  initscr();
  cbreak();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);

  int max_y, max_x;
  getmaxyx(stdscr, max_y, max_x);

  attron(A_REVERSE);
  mvprintw(0, 0, " SCANNING COMPLETE - Press any key to exit:  ");
  attroff(A_REVERSE);
  refresh();

  Rect root_bounds = {0, 1, (double)max_x, (double)(max_y - 1), -1};

  draw_node(root, root_bounds, 0);

  getch();
  endwin();
}
