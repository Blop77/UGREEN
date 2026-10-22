#ifndef NASWEEP_HASH_H
#define NASWEEP_HASH_H

#include <stddef.h>
#include <stdint.h>

#define FNV64_OFFSET 0xcbf29ce484222325ULL

uint64_t fnv1a64(const void *data, size_t len, uint64_t seed);

/* Hash a whole file's contents. Returns 0 on success, -1 on I/O error. */
int hash_file(const char *path, uint64_t *out);

/* Byte-for-byte comparison. Returns 1 if equal, 0 if not, -1 on error. */
int files_equal(const char *a, const char *b);

#endif
