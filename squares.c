/* squares.c */

#include <ncurses.h>
#include <ncurses/curses.h>
#include <stdio.h>
#include <stdlib.h>

#include "dir_list.h"

int draw_screen(DirectoryList *dirlist, unsigned long long total) {

  initscr();
  cbreak();
  noecho();
  curs_set(0);
  start_color();

  int rows, cols;
  getmaxyx(stdscr, rows, cols);

  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  wbkgd(stdscr, COLOR_PAIR(1));

  mvprintw(1, 2, "TOTAL DISK SIZE USED: %.2f GB",
           (double)total / 1024 / 1024 / 1024);

  refresh();

  int height = 10;
  int width = 20;
  int start_x = 10;
  int start_y = 5;

  WINDOW *block1 = newwin(height, width, start_y, start_x);

  init_pair(2, COLOR_WHITE, COLOR_BLUE);
  wbkgd(block1, COLOR_PAIR(2));

  box(block1, 0, 0);

  wrefresh(block1);

  getch();
  endwin();

  return 0;
}
