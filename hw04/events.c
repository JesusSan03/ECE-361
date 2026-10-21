#include "tally.h"
#include <stdio.h>
#include <string.h>

int main(void)
{
    char line[256];
    int skipped = 0;

    tally_t *t = tally_create();

    if (t == NULL) {
        fprintf(stderr, "Could not create tally\n");
        return 1;
    }

    while (fgets(line, sizeof(line), stdin) != NULL) {

        size_t len = strlen(line);

        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        } else if (len == sizeof(line) - 1) {
            /* Handle an input line longer than the buffer. */
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {
            }
        }

        if (line[0] == '\0' || line[0] == '#') {
            continue;
        }

        /* Count the event or record a refused event. */
        if (tally_add(t, line) == -1) {
            skipped++;
        }
    }

    tally_print(t, stdout);
    printf("lines skipped: %d\n", skipped);

    tally_destroy(t);

    return 0;
}