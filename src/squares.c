/* squares.c */
#define _GNU_SOURCE
#include <libgen.h>
#include <math.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCKS_NUM 10

WINDOW *blocks[BLOCKS_NUM];

int draw_screen() {

  initscr();
  cbreak();
  noecho();
  curs_set(0);
  start_color();

  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_WHITE, COLOR_BLUE);
  init_pair(3, COLOR_BLACK, COLOR_CYAN);

  int screen_max_y, screen_max_x;
  getmaxyx(stdscr, screen_max_y, screen_max_x);
  wbkgd(stdscr, COLOR_PAIR(1));

  refresh();

  int start_y = 2;
  int start_x = 1;
  int main_h = screen_max_y - 3;
  int main_w = screen_max_x - 2;

  blocks[0] = newwin(main_h, main_w, start_y, start_x);
  wbkgd(blocks[0], COLOR_PAIR(1));
  box(blocks[0], 0, 0);

  getch();

  for (int i = 0; i < BLOCKS_NUM; i++) {
    if (blocks[i])
      delwin(blocks[i]);
  }

  endwin();
  return 0;
}
