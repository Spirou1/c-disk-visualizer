/* scanner.c */

#define _DEFAULT_SOURCE

#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "dir_list.h"

unsigned long long list(const char *path, DirectoryList *dirlist) {

  unsigned long long total_bytes = 0;
  struct dirent *dent;
  struct stat s;
  DIR *dir;
  char full_path[PATH_MAX];

  if (strncmp(path, "/proc", 5) == 0 || strncmp(path, "/sys", 4) == 0 ||
      strncmp(path, "/dev", 4) == 0) {
    return 0;
  }

  dir = opendir(path);

  if (!dir) {
    switch (errno) {
    case EACCES:
      printf("EACCES ERROR OCCURRED\n");
      break;

    case ELOOP:
      printf("ELOOP ERROR OCCURRED\n");
      break;

    case ENAMETOOLONG:
      printf("ENAMETOOLONG ERROR OCCURRED\n");
      break;

    case ENOENT:
      printf("ENOENT ERROR OCCURRED\n");
      break;

    case ENOTDIR:
      printf("ENOTDIR ERROR OCCURRED\n");
      break;

    case EMFILE:
      printf("EMFILE ERROR OCCURRED\n");
      break;

    case ENFILE:
      printf("ENFILE ERROR OCCURRED\n");
      break;

    default:
      printf("UNDEFINED ERROR OCCURRED\n");
      break;
    }
    printf("ERROR WHILE OPENING DIRECTORY\n");
    return 1;
  }

  while ((dent = readdir(dir)) != NULL) {
    if (strcmp(dent->d_name, ".") == 0 || strcmp(dent->d_name, "..") == 0) {
      continue;
    }

    snprintf(full_path, sizeof(full_path), "%s/%s", path, dent->d_name);

    if (lstat(full_path, &s) == -1) {
      continue;
    }

    if (S_ISLNK(s.st_mode)) {
      total_bytes += s.st_size;
    } else if (S_ISDIR(s.st_mode)) {
      total_bytes += list(full_path, dirlist);
    } else if (S_ISREG(s.st_mode)) {
      total_bytes += s.st_size;
      //      printf("FILENAME: %s -------- SIZE ON DISK:%.2f MB\n",
      //      dent->d_name,
      //           s.st_size / (1024.0 * 1024.0));
    }
  }
  closedir(dir);

  DirectoryInfo file_info;
  file_info.path = strdup(path);
  file_info.bytes = total_bytes;

  dir_list_push(dirlist, file_info);

  return total_bytes;
}

int comp(const void *a, const void *b) {
  DirectoryInfo *dirinfoa = (DirectoryInfo *)a;
  DirectoryInfo *dirinfob = (DirectoryInfo *)b;

  if (dirinfoa->bytes > dirinfob->bytes) {
    return -1;
  }

  if (dirinfoa->bytes < dirinfob->bytes) {
    return 1;
  }

  return 0;
}

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

  dir_list_destroy(&dirlist);
  return 0;
}
