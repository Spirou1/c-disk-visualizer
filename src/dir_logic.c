/* dir_logic.c */

#define _GNU_SOURCE

#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dir_logic.h"

int directory_logic(char *current_folder, char *candidate_path) {

  if (strcmp(current_folder, candidate_path) == 0) {
    return 0;
  }

  char *path_copy = strdup(candidate_path);
  if (path_copy == NULL) {
    return 0;
  }

  char *parent = dirname(path_copy);

  int result = 0;

  // if candidate's father is current folder
  if (strcmp(current_folder, parent) == 0) {
    result = -1;
  }

  free(path_copy);
  return result;
}
