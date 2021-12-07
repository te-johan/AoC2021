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
    fread(file, size, 1, f);
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

uint64_t run(bool const_fuel) {
    char *numbers;
    uint32_t crabs[1000] = { 0 };
    int crab = 0;

    parse_input();
    
    /* load crab data */
    numbers = parse_get_line();
    for (;;) {
        char *next_num;
        int num;
        num = strtol(numbers, &next_num, 10);

        if (numbers == next_num) {
            break;
        }
        numbers = next_num;
        if (*numbers == ',') {
            numbers++;
        }
        crabs[crab++] = num;
    }

    uint32_t least_fuel = UINT32_MAX;
    for (int dest = 0; dest < 2000; dest++) {
        uint32_t fuel = 0;
        for (int i = 0; i < 1000; i ++) {
            uint32_t crab_fuel = dest > crabs[i] ? dest - crabs[i] : crabs[i] - dest;
            if (!const_fuel) {
                int i = crab_fuel;
                crab_fuel = 0;
                for (; i > 0; i--) {
                    crab_fuel += i;
                }
            }
            fuel += crab_fuel;
        }
        if (fuel < least_fuel) {
            least_fuel = fuel;
        }
    }
    return least_fuel;
}

void part1(void) {
    printf("part1: %lu\n", run(true));
}

void part2(void) {
    printf("part2: %lu\n", run(false));
}

int main(const char *argv[], int argc)
{
    part1();
    part2();
}
