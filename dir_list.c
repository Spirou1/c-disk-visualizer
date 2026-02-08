/* dir_list.c  */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dir_list.h"

DirectoryList init_dir_list() {
  DirectoryList self;
  self.capacity = 10;
  self.length = 0;
  self.data = malloc(self.capacity * sizeof(DirectoryInfo));
  return self;
}

DirectoryInfo *dir_list_at(DirectoryList *self, size_t i) {
  if (i < self->length) {
    return &self->data[i];
  }
  return NULL;
}

void dir_list_push(DirectoryList *self, DirectoryInfo item) {
  if (self->length >= self->capacity) {
    size_t new_capacity = self->capacity * 2;
    DirectoryInfo *new_data =
        realloc(self->data, new_capacity * sizeof(DirectoryInfo));

    if (new_data == NULL) {
      perror("Failed to reallocate memory");
      return;
    }

    self->data = new_data;
    self->capacity = new_capacity;
  }
  self->data[self->length] = item;
  self->length++;
}

void dir_list_pop(DirectoryList *self) {
  if (self->length > 0) {
    self->length--;
  }
}

void dir_list_delete(DirectoryList *self, size_t i) {
  if (i >= self->length)
    return;
  memmove(self->data + i, self->data + i + 1,
          (self->length - i - 1) * sizeof(DirectoryInfo));
  self->length--;
}

void dir_list_destroy(DirectoryList *self) {
  free(self->data);
  self->data = NULL;
  self->length = 0;
  self->capacity = 10;
}
