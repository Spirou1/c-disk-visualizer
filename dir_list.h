/* dir_list.h */

#ifndef DIRECTORY_LIST_H
#define DIRECTORY_LIST_H

#include <stddef.h>

typedef struct {
  char *path;
  unsigned long long bytes;
} DirectoryInfo;

typedef struct {
  DirectoryInfo *data;
  size_t length;
  size_t capacity;
} DirectoryList;

DirectoryList init_dir_list();
DirectoryInfo *dir_list_at(DirectoryList *self, size_t i);
void dir_list_push(DirectoryList *self, DirectoryInfo item);
void dir_list_pop(DirectoryList *self);
void dir_list_delete(DirectoryList *self, size_t i);
void dir_list_destroy(DirectoryList *self);

#endif
