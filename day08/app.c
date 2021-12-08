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

uint64_t run1(void) {
    char *line;
    int found = 0;

    parse_input();
    
    while ((line = parse_get_line()) != NULL) {
        line = strchr(line, '|');
        if (line == NULL) {
            break;
        }
        line += 2;

        int cnt = 0;
        for (;;) {
            if (*line != ' ' && *line != '\0') {
                cnt++;
            } else {
                if (cnt == 2 || cnt == 3 || cnt == 4 || cnt == 7) {
                    found++;
                }
                cnt = 0;
                if (*line == '\0') {
                    break;
                }
            }
            line++;
        }

    }

    return found;
}

bool is_superset(char *cmp, char *str)
{
    char c;
    while ((c = *str++) != '\0') {
        if (strchr(cmp, c) == NULL) {
            return false;
        }
    }
    return true;
}

char * sort_string(char *str)
{
    int n = strlen(str);

    for (int i = 0; i < n-1; i++) {
        for (int j = i+1; j < n; j++) {
            if (str[i] > str[j]) {
                char temp = str[i];
                str[i] = str[j];
                str[j] = temp;
            }
        }
    }
    return str;
}

uint64_t run2(void)
{
    char *line;
    char *first;
    char *sec;
    uint32_t count = 0;

    parse_input();
    
    while ((line = parse_get_line()) != NULL) {
        char *unknown[10] = { 0 };
        char *known[10] = { 0 };
        char *numbers[4] = { 0 };
        sec = strchr(line, '|');
        sec[0] = '\0'; // terminate first
        sec += 2;

        first = line;

        for (int i = 0; i < 10; i++) {
            *strchr(first, ' ') = '\0';
            unknown[i] = sort_string(first);
            while (*first++ != '\0');
        }
        for (int i = 0; i < 4; i++) {
            char *term = strchr(sec, ' ');
            if (term != NULL) {
                *term = '\0';
            }
            numbers[i] = sort_string(sec);
            while (*sec++ != '\0');
        }
        /* find fixed size */
        for (int i = 0; i < 10; i++) {
            if (unknown[i]) {
                if (strlen(unknown[i]) == 2) {
                    known[1] = unknown[i];
                    unknown[i] = NULL;
                }
                else if (strlen(unknown[i]) == 3) {
                    known[7] = unknown[i];
                    unknown[i] = NULL;
                }
                else if (strlen(unknown[i]) == 4) {
                    known[4] = unknown[i];
                    unknown[i] = NULL;
                }
                else if (strlen(unknown[i]) == 7) {
                    known[8] = unknown[i];
                    unknown[i] = NULL;
                }
            }
        }
        /* find 3 - len:5, superset of 1. */
        for (int i = 0; i < 10; i++) {
            if (unknown[i]) {
                if (strlen(unknown[i]) == 5 && is_superset(unknown[i], known[1])) {
                    known[3] = unknown[i];
                    unknown[i] = NULL;
                }
            }
        }
        /* find 9 - len:6, superset of 3. */
        for (int i = 0; i < 10; i++) {
            if (unknown[i]) {
                if (strlen(unknown[i]) == 6 && is_superset(unknown[i], known[3])) {
                    known[9] = unknown[i];
                    unknown[i] = NULL;
                }
            }
        }
        /* find 0 - len:6, superset of 1. */
        for (int i = 0; i < 10; i++) {
            if (unknown[i]) {
                if (strlen(unknown[i]) == 6 && is_superset(unknown[i], known[1])) {
                    known[0] = unknown[i];
                    unknown[i] = NULL;
                }
            }
        }
        /* find 6 - only unknown of len:6 left. */
        for (int i = 0; i < 10; i++) {
            if (unknown[i]) {
                if (strlen(unknown[i]) == 6 ) {
                    known[6] = unknown[i];
                    unknown[i] = NULL;
                }
            }
        }
         /* find the upper segment of 1 by comparing it to 6. */
        char upper_1 = known[1][0];
        if (strchr(known[6], upper_1)) {
            upper_1 = known[1][1];
        }
        /* find 2 - contains the upper segment of 1. */
        for (int i = 0; i < 10; i++) {
            if (unknown[i]) {
                if (strchr(unknown[i], upper_1)) {
                    known[2] = unknown[i];
                    unknown[i] = NULL; 
                }
            }
        }
        /* find 5 - only one left. */
        for (int i = 0; i < 10; i++) {
            if (unknown[i]) {
                known[5] = unknown[i];
                unknown[5] = NULL; 
            }
        }

        char digits[] = "    ";
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 10; j++) {
                if (strcmp(numbers[i], known[j]) == 0) {
                    digits[i] = '0' + j;
                }
            }
        }
        count += strtol(digits, NULL, 10);
    }

    return count;
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
