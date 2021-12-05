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
    file = malloc(size);
    fseek(f, 0L, SEEK_SET);
    fread(file, size, 1, f);
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

uint32_t get_res(uint8_t grid[1000][1000]) {
    uint32_t count = 0;
    for (int x = 0; x < 1000; x++) {
        for (int y = 0; y < 1000; y++) {
            if (grid[x][y] >= 2) {
                count++;
            }
        }
    }
    return count;
}

void write_line(uint8_t grid[1000][1000], int x1, int y1, int x2, int y2, bool with_diag) {
    if (x1 == x2) {
        int y_src = y1 < y2 ? y1 : y2;
        int y_dst = y1 < y2 ? y2 : y1;
        for (int y = y_src; y <= y_dst; y++) {
            grid[x1][y]++;
        }
    }
    else if (y1 == y2) {
        int x_src = x1 < x2 ? x1 : x2;
        int x_dst = x1 < x2 ? x2 : x1;
        for (int x = x_src; x <= x_dst; x++) {
            grid[x][y1]++;
        }
    } else if (with_diag) {
        if ((x1 > x2 && y1 > y2) || (x1 < x2 && y1 < y2)) {
            int x_src, x_dst, y;
            if (x1 < x2) {
                x_src = x1;
                x_dst = x2;
                y = y1;
            } else {
                x_src = x2;
                x_dst = x1;
                y = y2;
            }
            for (int x = x_src; x <= x_dst; x++) {
                grid[x][y++]++;
            }
        } else {
            int x_src, x_dst, y;
            if (x1 < x2) {
                x_src = x1;
                x_dst = x2;
                y = y1;
            } else {
                x_src = x2;
                x_dst = x1;
                y = y2;
            }
            for (int x = x_src; x <= x_dst; x++) {
                grid[x][y--]++;
            }
        }
    }
}

uint32_t run(bool with_diag) {
    uint8_t grid[1000][1000] = { 0 };
    char *line;

    parse_input();
    
    /* load board data */
    while (line = parse_get_line()) {
        int x1, x2, y1, y2;
        x1 = strtol(line, &line, 10);
        line++;
        y1 = strtol(line, &line, 10);
        line += 4;
        x2 = strtol(line, &line, 10);
        line++;
        y2 = strtol(line, &line, 10);
        write_line(grid, x1, y1, x2, y2, with_diag);
    }
    return get_res(grid);
}

void part1(void) {
    printf("part1: %u\n", run(false));
}

void part2(void) {
    printf("part2: %u\n", run(true));
}

int main(const char *argv[], int argc)
{
    part1();
    part2();
}
