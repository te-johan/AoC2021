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

uint32_t find_match(const char *str, char *lines[1000]) {
    uint32_t count = 0;
    for (int i = 0; i < 1000; i++) {
        if (strncmp(str, lines[i], strlen(str)) == 0) {
            count++;
        }
    }
    return count;
}

void solution2(void) {
    int i = 0;
    char *line;
    parse_input();
    char *lines[1000];
    while (line = parse_get_line()) {
        lines[i++] = line;
    }

    /**********************************/
    char res1[13] = { 0 };
    uint32_t cnt0, cnt1;
    for (i = 0; i < 12; i++) {
        res1[i] = '0';
        cnt0 = find_match(res1, lines);
        res1[i] = '1';
        cnt1 = find_match(res1, lines);
        if (!cnt0) {
            res1[i] = '1';
        } else if (!cnt1) {
            res1[i] = '0';
        } else {
            res1[i] = cnt1 >= cnt0 ? '1' : '0';
        }
    }
    /**********************************/
    char res2[13] = { 0 };
    for (i = 0; i < 12; i++) {
        res2[i] = '0';
        cnt0 = find_match(res2, lines);
        res2[i] = '1';
        cnt1 = find_match(res2, lines);
        if (!cnt0) {
            res2[i] = '1';
        } else if (!cnt1) {
            res2[i] = '0';
        } else {
            res2[i] = cnt1 >= cnt0 ? '0' : '1';
        }
    }
    printf("soltuion2 res:%lu\n", strtol(res1, NULL, 2) * strtol(res2, NULL, 2));
}

int main(const char *argv[], int argc)
{
    solution1();
    solution2();
}
