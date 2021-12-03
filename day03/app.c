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
    uint32_t count[12] = { 0 };
    uint32_t total_count = 0;
    uint32_t res = 0;
    parse_input();
    
    char *line;
    while (line = parse_get_line()) {
        for (int i = 0; i < 12; i++) {
            if (line[i] == '1') {
                count[i]++;
            }
        }
        total_count++;
    }
    for (int i = 0; i < 12; i++) {
        res |= count[i] > (total_count / 2) ? 1 << (11 - i) : 0;
    }

    printf("solution1 res:%d\n", res * ((~res) & 0xfff));
}

void solution2(void) {
}
int main(const char *argv[], int argc)
{
    solution1();
    solution2();
}