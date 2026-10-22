#define _XOPEN_SOURCE 700
#include "scan.h"

#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* nftw has no user-data pointer, so the callback writes here. */
static file_list *g_list;

static int push(file_list *list, const char *path, off_t size)
{
    if (list->count == list->cap) {
        size_t cap = list->cap ? list->cap * 2 : 256;
        file_entry *items = realloc(list->items, cap * sizeof *items);
        if (!items)
            return -1;
        list->items = items;
        list->cap = cap;
    }
    char *copy = strdup(path);
    if (!copy)
        return -1;
    list->items[list->count++] = (file_entry){ .path = copy, .size = size };
    return 0;
}

static int visit(const char *path, const struct stat *st, int type, struct FTW *ftw)
{
    (void)ftw;
    if (type == FTW_F && S_ISREG(st->st_mode))
        return push(g_list, path, st->st_size);
    if (type == FTW_DNR)
        fprintf(stderr, "nasweep: cannot read directory %s\n", path);
    return 0;
}

int scan_dir(const char *root, file_list *out)
{
    g_list = out;
    /* FTW_PHYS: don't follow symlinks, so we never count a file twice. */
    int rc = nftw(root, visit, 32, FTW_PHYS);
    g_list = NULL;
    return rc;
}

void file_list_free(file_list *list)
{
    for (size_t i = 0; i < list->count; i++)
        free(list->items[i].path);
    free(list->items);
    *list = (file_list){ 0 };
}
