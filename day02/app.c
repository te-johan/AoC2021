#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static char *file;
static char *line;

void parse_input(void)
{
    FILE *f = fopen("input", "r");
    long size;
    fseek(f, 0L, SEEK_END);
    size = ftell(f);
    file = malloc(size);
    fseek(f, 0L, SEEK_SET);
    fread(file, size, 1, f);
    line = file;
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

void solution1(void) {
    int x = 0;
    int y = 0;
    parse_input();
    
    char *line;
    while (line = parse_get_line()) {
        long val;
        if (*line == 'f') {
            x += atoi(line + sizeof("forward"));
        } else if (*line == 'u') {
            y -= atoi(line + sizeof("up"));
        } else if (*line == 'd') {
            y += atoi(line + sizeof("down"));
        }
        val = strtol(line, NULL, 10);

    }
    printf("solution1 x:%d y:%d res:%d\n", x, y, x * y);
}

void solution2(void) {
    uint32_t x = 0;
    uint32_t y = 0;
    uint32_t aim = 0;
    parse_input();
    
    char *line;
    while (line = parse_get_line()) {
        long val;
        if (*line == 'f') {
            uint32_t val = atoi(line + sizeof("forward"));
            y += val * aim;
            x += val;
        } else if (*line == 'u') {
            aim -= atoi(line + sizeof("up"));
        } else if (*line == 'd') {
            aim += atoi(line + sizeof("down"));
        }
        val = strtol(line, NULL, 10);

    }
    printf("solution2 x:%u y:%u res:%u\n", x, y, x * y);
}
int main(const char *argv[], int argc)
{
    solution1();
    solution2();
}