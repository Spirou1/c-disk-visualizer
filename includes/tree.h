/* tree.h  */

#ifndef TREE_H
#define TREE_H

#include <stddef.h>
#include <sys/types.h>

typedef struct {
  double x, y, w, h;
} Rect;

typedef struct Node {
  char *path;
  unsigned long long bytes;

  struct Node **children;
  size_t child_count;
  size_t child_cap;

  Rect rect;
} Node;

Node *create_node(const char *path);
void add_child(Node *parent, Node *child);
void free_tree(Node *node);
void sort_tree_by_size(Node *node);

#endif
