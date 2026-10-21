#include "../tally.h"
#include <stdio.h>
#include <string.h>
#include <stddef.h>

static int passed = 0;
static int failed = 0;

static void check(const char *name, int condition)
{
    printf("%s: %s\n", condition ? "PASS" : "FAIL", name);

    if (condition) {
        passed++;
    } else {
        failed++;
    }
}

int main(void)
{
    tally_t *t = tally_create();

    check("create tally", t != NULL);

    if (t == NULL) {
        return 1;
    }

    check("initial size", tally_size(t) == 0);

    check("add door", tally_add(t, "door") == 1);
    check("add window", tally_add(t, "window") == 1);
    check("add motion", tally_add(t, "motion") == 1);

    check("duplicate door", tally_add(t, "door") == 2);
    check("door count", tally_count(t, "door") == 2);
    check("window count", tally_count(t, "window") == 1);
    check("missing name", tally_count(t, "alarm") == 0);
    check("unique names", tally_size(t) == 3);

    /* Invalid names */
    check("empty name", tally_add(t, "") == -1);
    check("NULL name", tally_add(t, NULL) == -1);
    check("NULL tally", tally_add(NULL, "door") == -1);

    /* Name length limits */
    check("15 character name",
          tally_add(t, "abcdefghijklmno") == 1);

    check("16 character name rejected",
          tally_add(t, "abcdefghijklmnop") == -1);

    check("size unchanged after invalid name",
          tally_size(t) == 4);

    /* NULL argument handling */
    check("NULL size", tally_size(NULL) == 0);
    check("NULL count", tally_count(NULL, "door") == 0);
    check("NULL name count", tally_count(t, NULL) == 0);

    /* For the two independent tallies */
    tally_t *other = tally_create();

    check("second tally created", other != NULL);

    if (other != NULL) {
        check("second tally initially empty",
              tally_size(other) == 0);

        check("second tally independent",
              tally_add(other, "door") == 1 &&
              tally_count(t, "door") == 2);

        tally_destroy(other);
    }

    /* Checking the print order */
    FILE *out = tmpfile();

    if (out != NULL) {
        char line[128];
        char output[512] = "";

        tally_print(t, out);
        rewind(out);

        while (fgets(line, sizeof(line), out) != NULL) {
            if (strlen(output) + strlen(line) < sizeof(output)) {
                strcat(output, line);
            }
        }

        check("alphabetical print order",
              strcmp(output,
                     "abcdefghijklmno 1\n"
                     "door 2\n"
                     "motion 1\n"
                     "window 1\n") == 0);

        fclose(out);
    } else {
        check("alphabetical print order", 0);
    }

    /* Destroy and reuse */
    tally_destroy(t);

    tally_t *again = tally_create();

    check("reuse destroyed tally", again == t);
    check("reused tally is empty",
          tally_size(again) == 0);

    tally_destroy(again);
    tally_destroy(NULL);

    /* Test static pool capacity */
    tally_t *objects[TALLY_MAX];

    int all_created = 1;

    for (int i = 0; i < TALLY_MAX; i++) {
        objects[i] = tally_create();

        if (objects[i] == NULL) {
            all_created = 0;
        }
    }

    check("create four tallies", all_created);
    check("fifth tally refused", tally_create() == NULL);

    /* Fill a tally with 16 different names */
    if (objects[0] != NULL) {
        char name[16];

        for (int i = 0; i < TALLY_NAMES; i++) {
            snprintf(name, sizeof(name), "name%d", i);
            tally_add(objects[0], name);
        }

        check("full tally size",
              tally_size(objects[0]) == TALLY_NAMES);

        check("new name refused when full",
              tally_add(objects[0], "extra") == -1);

        check("existing name counts when full",
              tally_add(objects[0], "name0") == 2);
    }

    for (int i = 0; i < TALLY_MAX; i++) {
        tally_destroy(objects[i]);
    }

    printf("\nSummary: %d passed, %d failed\n",
           passed, failed);

    return failed == 0 ? 0 : 1;
}