#include "istack.h"

#include <stdio.h>
#include <stdlib.h>

static char matching_open(int c)
{
    if (c == ')') return '(';
    if (c == ']') return '[';
    return '{';
}

static char matching_close(int c)
{
    if (c == '(') return ')';
    if (c == '[') return ']';
    return '}';
}

static void advance_pos(int c, size_t *line, size_t *column)
{
    if (c == '\n') { ++*line; *column = 1; }
    else ++*column;
}

enum scan_state { NORMAL, SLASH, LINE_COMMENT, BLOCK_COMMENT,
                  BLOCK_STAR, SINGLE_QUOTE, DOUBLE_QUOTE,
                  SINGLE_ESCAPE, DOUBLE_ESCAPE };

int main(void)
{
    istack_t *s = istack_create();
    if (s == NULL) { fputs("error: out of memory\n", stderr); return 1; }

    enum scan_state state = NORMAL;
    size_t line = 1, column = 1, pairs = 0, deepest = 0;
    int c;
    while ((c = getchar()) != EOF) {
        size_t here_line = line, here_column = column;
        advance_pos(c, &line, &column);

        /* A slash in NORMAL is held until the next character identifies a
           comment; other next characters are handled normally. */
        if (state == SLASH) {
            if (c == '/') { state = LINE_COMMENT; continue; }
            if (c == '*') { state = BLOCK_COMMENT; continue; }
            state = NORMAL;
        }
        if (state == LINE_COMMENT) {
            if (c == '\n') state = NORMAL;
            continue;
        }
        if (state == BLOCK_COMMENT) {
            if (c == '*') state = BLOCK_STAR;
            continue;
        }
        if (state == BLOCK_STAR) {
            if (c == '/') state = NORMAL;
            else if (c != '*') state = BLOCK_COMMENT;
            continue;
        }
        if (state == SINGLE_ESCAPE) { state = SINGLE_QUOTE; continue; }
        if (state == DOUBLE_ESCAPE) { state = DOUBLE_QUOTE; continue; }
        if (state == SINGLE_QUOTE) {
            if (c == '\\') state = SINGLE_ESCAPE;
            else if (c == '\'') state = NORMAL;
            continue;
        }
        if (state == DOUBLE_QUOTE) {
            if (c == '\\') state = DOUBLE_ESCAPE;
            else if (c == '"') state = NORMAL;
            continue;
        }
        if (c == '/') { state = SLASH; continue; }
        if (c == '\'') { state = SINGLE_QUOTE; continue; }
        if (c == '"') { state = DOUBLE_QUOTE; continue; }
        if (c == '(' || c == '[' || c == '{') {
            if (istack_push(s, c) != 0) {
                fputs("error: out of memory\n", stderr);
                istack_destroy(s);
                return 1;
            }
            if (istack_count(s) > deepest) deepest = istack_count(s);
        } else if (c == ')' || c == ']' || c == '}') {
            int top;
            if (istack_peek(s, &top) != 0) {
                printf("line %zu, column %zu: found '%c', nothing open\n",
                       here_line, here_column, c);
                istack_destroy(s);
                return 1;
            }
            if (top != matching_open(c)) {
                printf("line %zu, column %zu: found '%c', expected '%c'\n",
                       here_line, here_column, c, matching_close(top));
                istack_destroy(s);
                return 1;
            }
            istack_pop(s, NULL);
            ++pairs;
        }
    }
    if (ferror(stdin)) {
        fputs("error: failed to read input\n", stderr);
        istack_destroy(s);
        return 1;
    }
    if (!istack_is_empty(s)) {
        int inner;
        istack_peek(s, &inner);
        printf("end of input: %zu still open, innermost '%c'\n",
               istack_count(s), inner);
        istack_destroy(s);
        return 1;
    }
    printf("balanced: %zu pairs, deepest %zu\n", pairs, deepest);
    istack_destroy(s);
    return 0;
}
