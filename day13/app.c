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

int x_max = 0;
int y_max = 0;

uint32_t count_dots(char grid[2000][2000])
{
    uint32_t dots = 0;
    for (int y = 0; y < y_max; y++) {
        for (int x = 0; x < x_max; x++) {
            if (grid[x][y] == '#') {
                dots++;
            }
        }
    }
    return dots;
}

void print_grind(char grid[2000][2000])
{
    for (int y = 0; y < y_max; y++) {
        for (int x = 0; x < x_max; x++) {
            printf("%c", grid[x][y]);
        }
        printf("\n");
    }
    printf("\n");
}

void fold(char grid[2000][2000], bool is_x_fold, int fold_offset)
{
    if (is_x_fold) {
        for (int x = fold_offset + 1; x < x_max; x++) {
            for (int y = 0; y < y_max; y++) {
                if (grid[x][y] == '#') {
                    grid[x - (x - fold_offset) * 2][y] = grid[x][y];
                }
            }
        }
        x_max = fold_offset;
    } else {
        for (int x = 0; x < x_max; x++) {
            for (int y = fold_offset + 1; y < y_max; y++) {
                if (grid[x][y] == '#') {
                    grid[x][y - (y - fold_offset) * 2] = grid[x][y];
                }
            }
        }
        y_max = fold_offset;
    }
}

int main(void)
{
    char *line;
    char grid[2000][2000];

    memset(grid, '.', sizeof(grid));

    parse_input();
    
    while ((line = parse_get_line()) != NULL) {
        int x, y;
        if (strcmp(line, "") == 0) {
            break;
        }
        x = strtol(line, &line, 10);
        y = strtol(line + 1, NULL, 10);
        grid[x][y] = '#';
        if (x > x_max) {
            x_max = x;
        }
        if (y > y_max) {
            y_max = y;
        }
    }
    x_max++;
    y_max++;
    bool part_one = true;

    while ((line = parse_get_line()) != NULL) {
        line += sizeof("fold along");
        bool x = *line == 'x' ? true : false;
        int fold_offset = strtol(line + 2, NULL, 10);

        fold(grid, x, fold_offset);
        if (part_one) {
            printf("part1: %u\n", count_dots(grid));
            part_one = false;
        }
    }
    printf("\npart2:\n");
    print_grind(grid);

    return 0;
}
