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

void flash(uint8_t grid[10][10], int x, int y)
{
    for (int _y = -1; _y <= 1; _y++) {
        for (int _x = -1; _x <= 1; _x++) {
            if (!_x && !_y)
                continue;
            if ((x + _x ) < 0 || (x + _x) > 9)
                continue;
            if ((y + _y ) < 0 || (y + _y) > 9)
                continue;
            if (grid[x + _x][y + _y]++ == 9)
                flash(grid, x + _x, y + _y);
        }
    }
}

uint32_t step(uint8_t grid[10][10])
{
    uint32_t flashes = 0;
    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
            if (grid[x][y]++ == 9) {
                flash(grid, x, y);
            }
        }
    }
    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
            if (grid[x][y] > 9) {
                grid[x][y] = 0;
                flashes++;
            }
        }
    }
    return flashes;
}

uint64_t part(int part)
{
    char *line;
    uint64_t result = 0;
    uint8_t grid[10][10];
    int x = 0;
    int y = 0;

    parse_input();
    
    while ((line = parse_get_line()) != NULL) {
        while (*line) {
            grid[x++][y] = *line++ - '0';
        }
        y++;
        x = 0;
    }
    if (part == 0) {
        for (int i = 1; i <= 100; i++) {
            result += step(grid);
        }
    } else {
        for (int i = 1; ; i++) {
            if (step(grid) == 100) {
                result = i;
                break;
            }
        }
    }
    return result;
}

int main(void)
{
    printf("part1: %lu\n", part(0));
    printf("part2: %lu\n", part(1));
}
