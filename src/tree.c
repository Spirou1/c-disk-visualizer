/* tree.c */
#define _DEFAULT_SOURCE

#include "tree.h"
#include <stdlib.h>
#include <string.h>

Node *create_node(const char *path) {
  Node *node = malloc(sizeof(Node));
  node->path = strdup(path);
  node->bytes = 0;
  node->children = NULL;
  node->child_count = 0;
  node->child_cap = 0;
  return node;
}

void add_child(Node *parent, Node *child) {
  if (parent->child_count == parent->child_cap) {
    if (parent->child_cap == 0) {
      parent->child_cap = 4;
    } else {
      parent->child_cap *= 2;
    }
    parent->children =
        realloc(parent->children, parent->child_cap * sizeof(Node *));
  }
  parent->children[parent->child_count++] = child;
}

int compare_nodes(const void *a, const void *b) {
  const Node *nodeA = *(const Node **)a;
  const Node *nodeB = *(const Node **)b;
  if (nodeA->bytes < nodeB->bytes) {
    return 1;
  }

  if (nodeA->bytes > nodeB->bytes) {
    return -1;
  }
  return 0;
}

void sort_tree_by_size(Node *node) {
  if (node->child_count > 0) {
    qsort(node->children, node->child_count, sizeof(Node *), compare_nodes);
    for (size_t i = 0; i < node->child_count; i++) {
      sort_tree_by_size(node->children[i]);
    }
  }
}

void free_tree(Node *node) {
  for (size_t i = 0; i < node->child_count; i++) {
    free_tree(node->children[i]);
  }
  free(node->children);
  free(node->path);
  free(node);
}
