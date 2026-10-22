#include "hash.h"

#include <stdio.h>
#include <string.h>

#define BUF_SIZE (64 * 1024)

uint64_t fnv1a64(const void *data, size_t len, uint64_t seed)
{
    const unsigned char *p = data;
    uint64_t h = seed;
    for (size_t i = 0; i < len; i++) {
        h ^= p[i];
        h *= 0x100000001b3ULL;
    }
    return h;
}

int hash_file(const char *path, uint64_t *out)
{
    FILE *f = fopen(path, "rb");
    if (!f)
        return -1;

    static unsigned char buf[BUF_SIZE];
    uint64_t h = FNV64_OFFSET;
    size_t n;
    while ((n = fread(buf, 1, sizeof buf, f)) > 0)
        h = fnv1a64(buf, n, h);

    int err = ferror(f);
    fclose(f);
    if (err)
        return -1;
    *out = h;
    return 0;
}

int files_equal(const char *a, const char *b)
{
    FILE *fa = fopen(a, "rb");
    FILE *fb = fopen(b, "rb");
    int result = -1;
    if (!fa || !fb)
        goto out;

    static unsigned char ba[BUF_SIZE], bb[BUF_SIZE];
    for (;;) {
        size_t na = fread(ba, 1, sizeof ba, fa);
        size_t nb = fread(bb, 1, sizeof bb, fb);
        if (na != nb || memcmp(ba, bb, na) != 0) {
            result = 0;
            break;
        }
        if (na == 0) {
            result = (ferror(fa) || ferror(fb)) ? -1 : 1;
            break;
        }
    }
out:
    if (fa)
        fclose(fa);
    if (fb)
        fclose(fb);
    return result;
}
