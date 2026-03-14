/* src/main.c */
#include <stdio.h>
#include <stdlib.h>

#include "scanner.h"
#include "squares.h"
#include "tree.h"

int main(int argc, char **argv) {
  char *path = (argc > 1) ? argv[1] : "/home/enzo";

  printf("Scanning %s, please wait...\n", path);

  Node *root = scan_dir(path);
  if (!root) {
    printf("Erro ao ler o diretório.\n");
    return 1;
  }

  sort_tree_by_size(root);

  draw_treemap(root);

  free_tree(root);

  return 0;
}
