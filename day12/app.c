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

#define MAX_CAVES 50
#define MAX_LINKS 20

struct cave {
    char *str;
    struct cave *links[MAX_LINKS];
    int link_cnt;
    bool big_cave;
    int in_path_cnt;
};

struct cave caves[MAX_CAVES] = { 0 };
struct cave *path[MAX_CAVES] = { 0 };
struct cave *end_cave = NULL;
int path_hops = 0;
bool (*allowed_jump)(struct cave *cave);
bool two_small_caves = false;

void push_hop(struct cave *cave)
{
    if (!cave->big_cave && cave->in_path_cnt == 1) {
        two_small_caves = true;
    }
    cave->in_path_cnt++;
    path[path_hops++] = cave;
}

void pop_hop(struct cave *cave)
{
    if (!cave->big_cave && cave->in_path_cnt == 2) {
        two_small_caves = false;
    }
    cave->in_path_cnt--;
    path[--path_hops] = NULL;
}

bool allowed_jump_part1(struct cave *cave)
{
    if (cave->big_cave) {
        return true;
    }
    if (cave->in_path_cnt) {
        return false;
    }
    return true;
}

bool allowed_jump_part2(struct cave *cave)
{
    if (cave->big_cave) {
        return true;
    }
    if (strcmp(cave->str, "start") == 0 && cave->in_path_cnt) {
        return false;
    }
    if (cave->in_path_cnt && two_small_caves) {
        return false;
    }
    return true;
}

struct cave * find_cave(char *cave_str)
{
    struct cave *cave;
    for (int i = 0; i < MAX_CAVES; i++) {
        if (caves[i].str && strcmp(caves[i].str, cave_str) == 0) {
            return &caves[i];
        }
    }
    return NULL;
}

struct cave * add_cave(char *cave_str)
{
    struct cave *new_cave = find_cave(cave_str);
    if (new_cave) {
        return new_cave;
    }
    for (int i = 0; i < MAX_CAVES; i++) {
        if (caves[i].str) {
            continue;
        }
        caves[i].str = cave_str;
        if (*cave_str >= 'A' && *cave_str <= 'Z') {
            caves[i].big_cave = true;
        }
        new_cave = &caves[i];
        break;
    }
    return new_cave;
}

void link_cave(struct cave *cave_a, struct cave *cave_b)
{
    cave_a->links[cave_a->link_cnt++] = cave_b;
    cave_b->links[cave_b->link_cnt++] = cave_a;
}

uint32_t explore(struct cave *cave)
{
    if (cave == end_cave) {
        return 1;
    }
    if (!allowed_jump(cave)) {
        return 0;
    }
    push_hop(cave);
    uint32_t count = 0;
    for (int i = 0; i < cave->link_cnt; i++) {
        count += explore(cave->links[i]);
    }
    pop_hop(cave);
    return count;
}

int main(void)
{
    char *line;

    parse_input();
    
    /* setup caves */
    while ((line = parse_get_line()) != NULL) {
        char *cave_a, *cave_b;
        cave_a = line;
        cave_b = strchr(line, '-');
        *cave_b++ = '\0';
        link_cave(add_cave(cave_a), add_cave(cave_b));
    }

    end_cave = find_cave("end");

    /* part 1 */
    allowed_jump = allowed_jump_part1;
    printf("part 1: %u\n", explore(find_cave("start")));

    /* part 2 */
    allowed_jump = allowed_jump_part2;
    printf("part 2: %u\n", explore(find_cave("start")));

    return 0;
}

