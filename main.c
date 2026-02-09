#include <stdio.h>
#include <stdlib.h>

#include "dir_list.h"
#include "scanner.h"
#include "squares.h"

int main(int argc, char **argv) {

  DirectoryList dirlist = init_dir_list();
  printf("Scanning your disk, please wait...\n");

  unsigned long long grand_total = list("/home/enzo", &dirlist);
  printf("\nGRAND TOTAL DISK SIZE: %.2f GB\n",
         (double)grand_total / 1024 / 1024 / 1024);
  qsort(dirlist.data, dirlist.length, sizeof(DirectoryInfo), comp);

  printf("15 BIGGEST DIRECTORIES LISTED: \n");
  for (int i = 0; i < 15; i++) {
    printf("DIRECTORY PATH: %s    || SIZE: %.2f GB\n", dirlist.data[i].path,
           (double)dirlist.data[i].bytes / 1024 / 1024 / 1024);
  }

  draw_screen(&dirlist, grand_total);

  dir_list_destroy(&dirlist);
  return 0;
}
