#include "ai.h"

#include <stdio.h>

int ai_explain(const dupe_report *report, const file_list *files, const char *endpoint)
{
    /*
     * TODO: POST the prompt below to `endpoint` (e.g.
     * http://localhost:8080/v1/chat/completions) with libcurl and print
     * the model's answer. For now we only show the prompt we would send.
     */
    printf("\n[ai] endpoint: %s (not wired up yet)\n", endpoint);
    printf("[ai] prompt:\n");
    printf("You are a storage assistant on a home NAS. Explain which of these "
           "duplicate files are safe to delete and why. Be brief.\n");
    for (size_t g = 0; g < report->count && g < 10; g++) {
        const dupe_group *grp = &report->groups[g];
        printf("- group %zu (%lld bytes each):", g + 1, (long long)grp->size);
        for (size_t k = 0; k < grp->n; k++)
            printf(" %s", files->items[grp->idx[k]].path);
        printf("\n");
    }
    return 0;
}
