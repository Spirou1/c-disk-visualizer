/* main.c  */

#include <stdio.h>
#include <stdlib.h>

#include "scanner.h"
#include "tree.h"

int main(int argc, char **argv) {

  printf("Scanning your disk, please wait...\n");

  char *path = "/home/enzo";
  Node *root = scan_dir(path);
  sort_tree_by_size(root);

  printf("Total scanned: %2.llu GB\n", root->bytes / 1000 / 1000 / 1000);

  printf("15 BIGGEST DIRECTORIES: \n");

  size_t limit = (root->child_count < 15) ? root->child_count : 15;

  for (size_t i = 0; i < limit; i++) {
    Node *child = root->children[i];
    printf("DIRECTORY: %s     ||SIZE: %llu GB\n", child->path,
           child->bytes / 1000 / 1000 / 1000);
  }

  free_tree(root);

  return 0;
}
