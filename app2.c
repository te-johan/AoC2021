#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static char *file;
static char *line;

void parse_input(void)
{
    FILE *f = fopen("input1", "r");
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

int main(const char *argv[], int argc)
{
    parse_input();
    
    int cnt = 0;
    long prev_val = INT32_MAX;
    char *line1, *line2, *line3;
    int done = 0;

    line1 = parse_get_line();
    line2 = parse_get_line();
    line3 = parse_get_line();

    for (;;) {
        long val = 0;
        if (!line3)
            break;
        val += strtol(line1, NULL, 10);
        val += strtol(line2, NULL, 10);
        val += strtol(line3, NULL, 10);

        if (val > prev_val) {
            cnt++;
        }
        prev_val = val;

        line1 = line2;
        line2 = line3;
        line3 = parse_get_line();
    }
    printf("input increased %d times\n", cnt);
}