#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

static char *file;
static char *line;

void parse_input(void)
{
    FILE *f = fopen("input", "r");
    long size;
    fseek(f, 0L, SEEK_END);
    size = ftell(f);
    if (file != NULL) {
        free(file);
    }
    file = malloc(size + 1);
    fseek(f, 0L, SEEK_SET);
    if (fread(file, size, 1, f) < 1) {
        printf("fread failed\n");
    }
    file[size] = '\0';
    line = file;
    fclose(f);
}

char * parse_get_line()
{
    char *ret = line;
    if (!line)
        return NULL;
    line = strchr(line, '\n');
    if (line) {
        *line++ = '\0';
    }
    return ret;
}

struct dot {
    int x;
    int y;
    struct dot *next;
};

struct dot * find_dot(struct dot *root, int x, int y)
{
    while (root) {
        if (root->x == x && root->y == y) {
            return root;
        }
        root = root->next;
    }
    return NULL;
}

uint32_t count_lit(struct dot *root) {
    uint32_t lit = 0;
    while (root) {
        lit++;
        root = root->next;
    }
    return lit;
}

uint8_t enhance(char *algo, struct dot *root, int x, int y, bool inverted)
{
    uint32_t index = 0;
    for (int y_diff = -1; y_diff <= 1; y_diff++) {
        for (int x_diff = -1; x_diff <= 1; x_diff++) {
            struct dot *dot = find_dot(root, x + x_diff, y + y_diff);
            if (inverted) {
                index = index << 1 | !dot;
            } else {
                index = index << 1 | !!dot;
            }
        }
    }
    return algo[index];
}

void new_dot(struct dot **root, int x, int y)
{
    struct dot *dot = malloc(sizeof(struct dot));
    dot->x = x;
    dot->y = y;
    dot->next = *root;
    *root = dot;
}

void reset_dots(struct dot **root)
{
    while(*root) {
        struct dot *tmp = (*root)->next;
        free(*root);
        *root = tmp;
    }
}

int main(void)
{
    char *line;
    parse_input();
    struct dot *root = NULL;

    char *algo = parse_get_line();

    (void)parse_get_line();

    int y = 0;
    int x = 0;
    while ((line = parse_get_line()) != NULL) {
        while (*line) {
            if (*line == '#') {
                new_dot(&root, x, y);
            }
            line++;
            x++;
        }
        y++;
        x = 0;
    }
    struct dot *enhanced = NULL;
    for (int i = 0; i < 25; i ++) {
        reset_dots(&enhanced);
        for (int y = -50; y < 150; y++) {
            for (int x = -50; x < 150; x++) {
                char c = enhance(algo, root, x, y, false);
                if (c == '.') {
                    new_dot(&enhanced, x, y);
                }
            }
        }

        reset_dots(&root);
        for (int y = -50; y < 150; y++) {
            for (int x = -50; x < 150; x++) {
                char c = enhance(algo, enhanced, x, y, true);
                if (c == '#') {
                    new_dot(&root, x, y);
                }
            }
        }
        if (i == 0) {
            printf("part 1: %d\n", count_lit(root));
        }
    }
    printf("part 2: %d\n", count_lit(root));

    return 0;
}
