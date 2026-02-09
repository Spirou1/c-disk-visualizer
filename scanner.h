#ifndef SCANNER_H
#define SCANNER_H

#include "dir_list.h"

extern unsigned long long total_bytes;
unsigned long long list(const char *path, DirectoryList *dirlist);
int comp(const void *a, const void *b);

#endif
