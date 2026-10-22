#ifndef NASWEEP_SCAN_H
#define NASWEEP_SCAN_H

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

typedef struct {
    char *path;
    off_t size;
    uint64_t hash;
    int hashed;
} file_entry;

typedef struct {
    file_entry *items;
    size_t count;
    size_t cap;
} file_list;

/* Recursively collect every regular file under root. Returns 0 on success. */
int scan_dir(const char *root, file_list *out);
void file_list_free(file_list *list);

#endif
