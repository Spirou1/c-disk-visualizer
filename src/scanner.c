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

#include "scanner.h"
#include "tree.h"

Node *scan_dir(const char *path) {

  Node *current_node = create_node(path);

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
    return current_node;
  }

  while ((dent = readdir(dir)) != NULL) {
    if (strcmp(dent->d_name, ".") == 0 || strcmp(dent->d_name, "..") == 0) {
      continue;
    }

    snprintf(full_path, sizeof(full_path), "%s/%s", path, dent->d_name);

    if (lstat(full_path, &s) == -1) {
      continue;
    }

    if (S_ISDIR(s.st_mode)) {
      Node *sub_dir = scan_dir(full_path);
      current_node->bytes += sub_dir->bytes;
      add_child(current_node, sub_dir);
    } else if (S_ISREG(s.st_mode)) {
      current_node->bytes += s.st_blocks * 512;
    }
  }
  closedir(dir);

  return current_node;
}
