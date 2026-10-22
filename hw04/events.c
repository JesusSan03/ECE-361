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

        /* Remove newline or detect an overlong line. */
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        } else if (len == sizeof(line) - 1) {
            int ch = getchar();

            if (ch != '\n' && ch != EOF) {
                while ((ch = getchar()) != '\n' && ch != EOF) {
                }
                skipped++;
                continue;
            }
        }

        /* Ignore empty lines and comments. */
        if (line[0] == '\0' || line[0] == '#') {
            continue;
        }

        /* Add the name, or count it as skipped. */
        if (tally_add(t, line) == -1) {
            skipped++;
        }
    }

    /* Print names alphabetically with their counts. */
    tally_print(t, stdout);

    printf("names: %d, lines skipped: %d\n",
           tally_size(t), skipped);

    tally_destroy(t);

    return 0;
}