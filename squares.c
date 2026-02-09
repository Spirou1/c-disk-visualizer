#include <ncurses.h>
#include <ncurses/curses.h>
#include <stdio.h>
#include <stdlib.h>

#include "dir_list.h"
#include "scanner.h"

int draw_screen() {

  initscr();
  cbreak();
  noecho();
  curs_set(0);
  start_color();

  init_pair(1, COLOR_WHITE, COLOR_BLUE);

  int yMax, xMax;
  getmaxyx(stdscr, yMax, xMax);

  int height = 10;
  int width = 20;
  int start_y = (yMax - height) / 2;
  int start_x = (xMax - width) / 2;

  refresh();

  endwin();

  return 0;
}
