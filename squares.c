/* squares.c */

#include <ncurses.h>
#include <ncurses/curses.h>
#include <stdio.h>
#include <stdlib.h>

#include "dir_list.h"

#define BLOCKS_NUM 30
WINDOW *blocks[BLOCKS_NUM];

int draw_screen(DirectoryList *dirlist, unsigned long long total) {

  initscr();
  cbreak();
  noecho();
  curs_set(0);
  start_color();

  int max_y, max_x;
  getmaxyx(stdscr, max_y, max_x);

  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_WHITE, COLOR_BLUE);

  wbkgd(stdscr, COLOR_PAIR(1));

  mvprintw(1, 2, "TOTAL DISK SIZE USED: %.2f GB",
           (double)total / 1024 / 1024 / 1024);

  refresh();

  int start_x = 10;
  int start_y = 5;
  int height = 10;

  for (int i = 0; i < BLOCKS_NUM; i++) {
    double ratio = (double)dirlist->data[i].bytes / total * 2;
    int width = (int)(ratio * max_y);

    if (start_x + width > max_x) {
      start_y += height;
      start_x = 10;
    }

    if (width < 10) {
      width = 20;
    }
    blocks[i] = newwin(height, width, start_y, start_x);
    wbkgd(blocks[i], COLOR_PAIR(2));
    box(blocks[i], 0, 0);
    mvwprintw(blocks[i], 1, 1, "NAME: %s", dirlist->data[i].path);
    wrefresh(blocks[i]);
    start_x += width + 1;
  }

  getch();

  for (int i = 0; i < BLOCKS_NUM; i++) {
    delwin(blocks[i]);
  }

  endwin();

  return 0;
}
