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

struct rule {
    uint64_t count;
    uint64_t count_to_add;
    char a;
    char b;
    char to_insert;
};

uint32_t rule_count = 0;
struct rule rules[200] = { 0 };
uint64_t count[128] = { 0 };
char last_char;

uint64_t res(void) 
{
    memset(count, 0, sizeof(count));
    for (int i = 0; i < rule_count; i++) {
        count[rules[i].a] += rules[i].count;
    }
    // only count first char of pair.
    // need to add the last char as well.
    count[last_char]++;

    uint64_t max = 0;
    uint64_t min = UINT64_MAX;
    for (int i = 0; i < 128; i++) {
        if (count[i] > max) {
            max = count[i];
        }
        if (count[i] > 0 && count[i] < min) {
            min = count[i];
        }
    }
    return max - min;
}

struct rule * find_rule(char a, char b)
{
    for (int i = 0; i < rule_count; i++) {
        if (a == rules[i].a && b == rules[i].b) {
            return &rules[i];
        }
    }
    return NULL;
}

int main(void)
{
    char *line, *start;
    struct entry *last = NULL;

    parse_input();
    
    start = parse_get_line();

    parse_get_line(); // skip newline

    /* read rules */
    while ((line = parse_get_line()) != NULL) {
        rules[rule_count].a = line[0];
        rules[rule_count].b = line[1];
        rules[rule_count].to_insert = line[6];
        rule_count++;
    }

    for (char *str = start; *(str + 1) != '\0'; str++) {
        char a = *str;
        char b = *(str + 1);
        find_rule(a, b)->count++;
        last_char = *(str + 1);
    }
    for (int i = 1; i <= 40; i++) {
        for (int i = 0; i < rule_count; i++) {
            struct rule *rule = &rules[i];
            if (rule->count > 0) {
                find_rule(rule->a, rule->to_insert)->count_to_add += rule->count;
                find_rule(rule->to_insert, rule->b)->count_to_add += rule->count;
                rule->count = 0;
            }
        }
        for (int i = 0; i < rule_count; i++) {
            struct rule *rule = &rules[i];
            rule->count += rule->count_to_add;
            rule->count_to_add = 0;
        }
        if (i == 10) {
            printf("part1: %lu\n", res());
        }
    }
    printf("part2: %lu\n", res());


    return 0;
}
