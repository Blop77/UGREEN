#ifndef NASWEEP_DUPES_H
#define NASWEEP_DUPES_H

#include "scan.h"

/* A set of files with identical contents. idx points into the file_list. */
typedef struct {
    size_t *idx;
    size_t n;
    off_t size;
} dupe_group;

typedef struct {
    dupe_group *groups;
    size_t count;
    uint64_t wasted_bytes; /* bytes freed if all but one copy were removed */
} dupe_report;

/* Sorts files in place, then finds exact duplicates. Returns 0 on success. */
int find_dupes(file_list *files, dupe_report *out);
void dupe_report_free(dupe_report *report);

#endif
