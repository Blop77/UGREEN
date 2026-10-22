#include "ai.h"
#include "dupes.h"
#include "scan.h"

#include <stdio.h>
#include <string.h>

#define NASWEEP_VERSION "0.1.0"

static void usage(FILE *out)
{
    fprintf(out,
            "usage: nasweep [--json] [--ai URL] <directory>\n"
            "\n"
            "  --json     print the report as JSON\n"
            "  --ai URL   ask a local LLM at URL to explain the report\n"
            "  --version  print version and exit\n");
}

static void json_string(const char *s)
{
    putchar('"');
    for (; *s; s++) {
        unsigned char c = (unsigned char)*s;
        if (c == '"' || c == '\\')
            printf("\\%c", c);
        else if (c < 0x20)
            printf("\\u%04x", c);
        else
            putchar(c);
    }
    putchar('"');
}

static void print_text(const dupe_report *r, const file_list *files)
{
    for (size_t g = 0; g < r->count; g++) {
        const dupe_group *grp = &r->groups[g];
        printf("group %zu: %zu copies, %lld bytes each\n", g + 1, grp->n, (long long)grp->size);
        for (size_t k = 0; k < grp->n; k++)
            printf("  %s\n", files->items[grp->idx[k]].path);
    }
    printf("\n%zu files scanned, %zu duplicate groups, %llu bytes reclaimable\n",
           files->count, r->count, (unsigned long long)r->wasted_bytes);
}

static void print_json(const dupe_report *r, const file_list *files)
{
    printf("{\"files_scanned\":%zu,\"wasted_bytes\":%llu,\"groups\":[",
           files->count, (unsigned long long)r->wasted_bytes);
    for (size_t g = 0; g < r->count; g++) {
        const dupe_group *grp = &r->groups[g];
        printf("%s{\"size\":%lld,\"paths\":[", g ? "," : "", (long long)grp->size);
        for (size_t k = 0; k < grp->n; k++) {
            if (k)
                putchar(',');
            json_string(files->items[grp->idx[k]].path);
        }
        printf("]}");
    }
    printf("]}\n");
}

int main(int argc, char **argv)
{
    int json = 0;
    const char *ai_url = NULL;
    const char *root = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--json") == 0) {
            json = 1;
        } else if (strcmp(argv[i], "--ai") == 0 && i + 1 < argc) {
            ai_url = argv[++i];
        } else if (strcmp(argv[i], "--version") == 0) {
            printf("nasweep %s\n", NASWEEP_VERSION);
            return 0;
        } else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            usage(stdout);
            return 0;
        } else if (argv[i][0] != '-' && !root) {
            root = argv[i];
        } else {
            usage(stderr);
            return 2;
        }
    }
    if (!root) {
        usage(stderr);
        return 2;
    }

    file_list files = { 0 };
    if (scan_dir(root, &files) != 0) {
        perror("nasweep: scan failed");
        file_list_free(&files);
        return 1;
    }

    dupe_report report;
    if (find_dupes(&files, &report) != 0) {
        fprintf(stderr, "nasweep: out of memory\n");
        file_list_free(&files);
        return 1;
    }

    if (json)
        print_json(&report, &files);
    else
        print_text(&report, &files);

    if (ai_url)
        ai_explain(&report, &files, ai_url);

    dupe_report_free(&report);
    file_list_free(&files);
    return 0;
}
