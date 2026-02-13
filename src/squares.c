/* squares.c */

#define _GNU_SOURCE

#include <libgen.h>
#include <ncurses.h>
#include <ncurses/curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dir_list.h"
#include "dir_logic.h"

#define BLOCKS_NUM 50
WINDOW *blocks[BLOCKS_NUM];

int dir_logic(DirectoryList *dirlist);

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
  init_pair(3, COLOR_BLACK, COLOR_MAGENTA);

  char *current_path = "/home/enzo";

  wbkgd(stdscr, COLOR_PAIR(1));

  mvprintw(1, 2, "TOTAL DISK SIZE USED: %.2f GB",
           (double)total / 1000 / 1000 / 1000);

  refresh();

  int start_x = 10;
  int start_y = 5;
  int height = 10;
  int header_h = 4;
  int footer_h = 16;
  int footer_w = 80;
  int footer_start_y = max_y - footer_h;
  int footer_start_x = max_x - footer_w;
  int footer_limit_y = max_y - footer_h;
  int footer_limit_x = max_x - footer_w;

  WINDOW *win_header = newwin(header_h, max_x, 0, 0);
  WINDOW *win_main = newwin(max_y - header_h, max_x, header_h, 0);
  WINDOW *win_footer =
      newwin(footer_h, footer_w, footer_start_y, footer_start_x);

  box(win_header, 0, 0);
  box(win_main, 0, 0);
  box(win_footer, 0, 0);
  wbkgd(win_footer, COLOR_PAIR(2));
  wbkgd(win_header, COLOR_PAIR(3));

  const char *titulo = "DISK USAGE ANALYZER";
  int largura_janela = getmaxx(win_header);
  int pos_x = (largura_janela - strlen(titulo)) / 2;

  mvwprintw(win_header, 1, pos_x, "%s", titulo);

  refresh();
  wrefresh(win_header);
  wrefresh(win_main);

  unsigned long long local_sum = 0;
  for (int k = 0; k < BLOCKS_NUM; k++) {
    if (directory_logic(current_path, dirlist->data[k].path) == 1) {
      local_sum += dirlist->data[k].bytes;
    }
  }
  if (local_sum == 0) {
    local_sum = 1;
  }

  for (int i = 0; i < BLOCKS_NUM; i++) {
    double ratio = (double)dirlist->data[i].bytes / local_sum;
    int width = (int)(ratio * (max_x - 4));

    if (directory_logic(current_path, dirlist->data[i].path) == 0) {
      continue;
    }

    if (width < 10)
      width = 20;
    if (width > max_x - 4)
      width = max_x - 4;

    if (start_x + width > max_x - 2) {
      start_y += height;
      start_x = 2;
    }

    if (start_y + height > footer_limit_y && start_x + width > footer_limit_x) {
      start_y += height;
      start_x = 2;
    }

    if (start_y + height >= max_y) {
      break;
    }

    blocks[i] = newwin(height, width, start_y, start_x);
    wbkgd(blocks[i], COLOR_PAIR(2));
    box(blocks[i], 0, 0);
    mvwprintw(blocks[i], 1, 1, "NAME: %s", dirlist->data[i].path);
    wrefresh(blocks[i]);
    start_x += width + 1;
  }

  wrefresh(win_footer);
  getch();

  for (int i = 0; i < BLOCKS_NUM; i++) {
    delwin(blocks[i]);
  }

  endwin();

  return 0;
}
