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

static char stack[256];
static int stack_idx = 0;

static void push_stack(char c)
{
    stack[stack_idx++] = c;
}

static char pop_stack()
{
    return stack[--stack_idx];
}

static char get_exp(char c) {
    if (c == '(')      return ')';
    else if (c == '[') return ']';
    else if (c == '{') return '}';
    else if (c == '<') return '>';
    else return printf("error"), '0';
}

static uint32_t get_score(char c) {
    if (c == ')')      return 3;
    else if (c == ']') return 57;
    else if (c == '}') return 1197;
    else if (c == '>') return 25137;
    else return printf("error"), 0;
}

uint32_t check_syntax(char *str)
{
    char c, exp;
    stack_idx = 0;
    while (*str) {
        c = *str++;
        if (c == '(' || c == '[' || c == '{' || c == '<') {
            push_stack(c);
            continue;
        } else {
            exp = get_exp(pop_stack());
            if (c != exp) {
                return get_score(c);
            }
        }
    }
    return 0;
}

uint64_t run1(void)
{
    char *line;
    uint32_t score = 0;

    parse_input();
    
    while ((line = parse_get_line()) != NULL) {
        score = check_syntax(line);
    }

    return score;
}

uint64_t count_incomplete(char *str)
{
    char c, exp;
    stack_idx = 0;
    while (*str) {
        c = *str++;
        if (c == '(' || c == '[' || c == '{' || c == '<') {
            push_stack(c);
            continue;
        } else {
            exp = get_exp(pop_stack());
            if (c != exp) {
                return 0;
            }
        }
    }
    uint64_t score = 0;
    while (stack_idx) {
        score *= 5;
        c = get_exp(pop_stack());
        switch (c) {
            case ')': score += 1; break;
            case ']': score += 2; break;
            case '}': score += 3; break;
            case '>': score += 4; break;
        }
    }
    return score;
}

uint64_t run2(void)
{
    char *line;

    parse_input();
    
    int score_idx = 0;
    uint64_t scores[150] = { 0 };
    while ((line = parse_get_line()) != NULL) {
        uint64_t val = count_incomplete(line);
        if (val) {
            score_idx++;
            for (int i = 0; i < score_idx; i++) {
                if (val > scores[i]) {
                    for (int j = score_idx; j > i; j--) {
                        scores[j] = scores[j - 1];
                    }
                    scores[i] = val;
                    break;
                }
            }
        }
    }
    return scores[(score_idx/2)];
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
