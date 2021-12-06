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

uint64_t run(uint64_t days) {
    uint64_t fish[9] = { 0 };
    char *numbers;

    parse_input();
    
    /* load fish data */
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
        fish[num]++;
    }

    for (int i = 0; i < days; i++) {
        uint64_t birthing_fish = fish[0];
        memmove(&fish[0], &fish[1], sizeof(fish) - sizeof(fish[0]));
        fish[6] += birthing_fish;
        fish[8] = birthing_fish;
    }

    uint64_t total = 0;
    for (int i = 0; i < 9; i++) {
        total += fish[i];
    }
    return total;
}

void part1(void) {
    printf("part1: %lu\n", run(80));
}

void part2(void) {
    printf("part2: %lu\n", run(256));
}

int main(const char *argv[], int argc)
{
    part1();
    part2();
}
