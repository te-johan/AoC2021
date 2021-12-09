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

bool check_adjacent(uint8_t grid[100][100], int row, int col)
{
    bool low_point = true;
    if (row > 0 && grid[row - 1][col] <= grid[row][col]) {
        low_point = false;
    }
    if (row < 99 && grid[row + 1][col] <= grid[row][col]) {
        low_point = false;
    }
    if (col > 0 && grid[row][col - 1] <= grid[row][col]) {
        low_point = false;
    }
    if (col < 99 && grid[row][col + 1] <= grid[row][col]) {
        low_point = false;
    }
    return low_point;
}

uint64_t run1(void)
{
    static uint8_t grid[100][100] = { 0 };
    char *line;
    int count = 0;
    int row = 0;
    int col = 0;

    parse_input();
    
    while ((line = parse_get_line()) != NULL) {
        col = 0;
        while (*line != '\0')  {
            grid[row][col++] = *line - '0';
            line++;
        }
        row++;
    }

    for (row = 0; row < 100; row++) {
        for (col = 0; col < 100; col++) {
            if (check_adjacent(grid, row, col)) {
                count += grid[row][col] + 1;
            }
        }
    }
    return count;
}

bool checked[100][100] = { 0 };

uint32_t check_higher(uint8_t grid[100][100], int row, int col)
{
    uint32_t count = 1;
    if (checked[row][col]) {
        return 0;
    }
    checked[row][col] = true;

    if (row > 0 && grid[row - 1][col] < 9 && grid[row - 1][col] > grid[row][col]) {
        count += check_higher(grid, row - 1, col);
    }
    if (row < 99 && grid[row + 1][col] < 9 && grid[row + 1][col] > grid[row][col]) {
        count += check_higher(grid, row + 1, col);
    }
    if (col > 0 && grid[row][col - 1] < 9 && grid[row][col - 1] > grid[row][col]) {
        count += check_higher(grid, row, col - 1);
    }
    if (col < 99 && grid[row][col + 1] < 9 && grid[row][col + 1] > grid[row][col]) {
        count += check_higher(grid, row, col + 1);
    }
    return count;
}

uint64_t run2(void)
{
    uint32_t top[3] = { 0 };
    static uint8_t grid[100][100] = { 0 };
    char *line;
    int count = 0;
    int row = 0;
    int col = 0;

    parse_input();
    
    while ((line = parse_get_line()) != NULL) {
        col = 0;
        while (*line != '\0')  {
            grid[row][col++] = *line - '0';
            line++;
        }
        row++;
    }

    for (row = 0; row < 100; row++) {
        for (col = 0; col < 100; col++) {
            if (check_adjacent(grid, row, col)) {
                uint32_t val = check_higher(grid, row, col);
                if (val > top[0]) {
                    top[2] = top[1];
                    top[1] = top[0];
                    top[0] = val;
                } else if (val > top[1]) {
                    top[2] = top[1];
                    top[1] = val;
                } else if (val > top[2]) {
                    top[2] = val;
                }
            }
        }
    }
    return top[0] * top[1] * top[2];
}

void part1(void)
{
    printf("part1: %lu\n", run1());
}

void part2(void)
{
    printf("part2: %lu\n", run2());
}

int main(void)
{
    part1();
    part2();
}
