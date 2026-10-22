#include "dupes.h"
#include "hash.h"

#include <stdio.h>
#include <stdlib.h>

static int by_size(const void *a, const void *b)
{
    const file_entry *x = a, *y = b;
    return (x->size > y->size) - (x->size < y->size);
}

static int by_hash(const void *a, const void *b)
{
    const file_entry *x = a, *y = b;
    if (x->hashed != y->hashed)
        return y->hashed - x->hashed; /* hashed files first */
    return (x->hash > y->hash) - (x->hash < y->hash);
}

static int add_group(dupe_report *r, file_entry *base, size_t start, size_t end)
{
    size_t *idx = malloc((end - start) * sizeof *idx);
    dupe_group *groups = realloc(r->groups, (r->count + 1) * sizeof *groups);
    if (!idx || !groups) {
        free(idx);
        return -1;
    }
    r->groups = groups;

    /* Confirm byte-for-byte against the first file so a hash collision
     * can never cause a false "duplicate". */
    size_t n = 0;
    idx[n++] = start;
    for (size_t i = start + 1; i < end; i++)
        if (files_equal(base[start].path, base[i].path) == 1)
            idx[n++] = i;

    if (n < 2) {
        free(idx);
        return 0;
    }
    r->groups[r->count++] = (dupe_group){ .idx = idx, .n = n, .size = base[start].size };
    r->wasted_bytes += (uint64_t)base[start].size * (n - 1);
    return 0;
}

int find_dupes(file_list *files, dupe_report *out)
{
    *out = (dupe_report){ 0 };
    file_entry *f = files->items;
    qsort(f, files->count, sizeof *f, by_size);

    /* Only files that share a size can be duplicates, so we only hash those. */
    size_t i = 0;
    while (i < files->count) {
        size_t j = i + 1;
        while (j < files->count && f[j].size == f[i].size)
            j++;

        if (j - i > 1 && f[i].size > 0) {
            for (size_t k = i; k < j; k++) {
                if (hash_file(f[k].path, &f[k].hash) == 0)
                    f[k].hashed = 1;
                else
                    fprintf(stderr, "nasweep: cannot read %s\n", f[k].path);
            }
            qsort(f + i, j - i, sizeof *f, by_hash);

            size_t a = i;
            while (a < j && f[a].hashed) {
                size_t b = a + 1;
                while (b < j && f[b].hashed && f[b].hash == f[a].hash)
                    b++;
                if (b - a > 1 && add_group(out, f, a, b) != 0)
                    return -1;
                a = b;
            }
        }
        i = j;
    }
    return 0;
}

void dupe_report_free(dupe_report *report)
{
    for (size_t i = 0; i < report->count; i++)
        free(report->groups[i].idx);
    free(report->groups);
    *report = (dupe_report){ 0 };
}
