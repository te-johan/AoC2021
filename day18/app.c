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

typedef struct pair {
    int left;
    int right;
    struct pair *left_pair;
    struct pair *right_pair;
    struct pair *parent;
    int allocated;
} pair_t;

#define PAIR_ENTRIES 1000

pair_t pair_array[PAIR_ENTRIES];
pair_t pair_save[PAIR_ENTRIES];

pair_t * alloc_pair(void)
{
    int i;
    for (i = 0; i < PAIR_ENTRIES; i++) {
        if (!pair_array[i].allocated) {
            break;
        }
    }
    if (i == PAIR_ENTRIES) {
        abort();
    }
    pair_array[i].allocated = true;
    return &pair_array[i];
}

void free_pair(pair_t *pair)
{
    memset(pair, 0, sizeof(pair_t));
}

void save_pairs(void) {
    memcpy(pair_save, pair_array, sizeof(pair_array));
}

void restore_pairs(void) {
    memcpy(pair_array, pair_save, sizeof(pair_array));
}

pair_t * parse_pair(char **str, pair_t *parent)
{
    pair_t * pair = alloc_pair();
    pair->parent = parent;
    (*str)++;
    if (**str == '[') {
        pair->left_pair = parse_pair(str, pair);

    } else {
        pair->left = strtol(*str, str, 10);
    }
    (*str)++;
    if (**str == '[') {
        pair->right_pair = parse_pair(str, pair);
    } else {
        pair->right = strtol(*str, str, 10);
    }
    (*str)++;

    return pair;
}

pair_t * add_pair(pair_t *pair1, pair_t *pair2)
{
    pair_t *pair = alloc_pair();
    pair->left_pair = pair1;
    pair->right_pair = pair2;
    pair1->parent = pair;
    pair2->parent = pair;
    return pair;
}

void find_and_add_left_number(pair_t *pair, int value) {
    pair_t *trail = pair->parent;
    while (trail->left_pair == pair) {
        pair = trail;
        trail = trail->parent;
        if (!trail) {
            return;
        }
    }
    if (trail->left_pair) {
        trail = trail->left_pair;
        while (trail->right_pair) {
            trail = trail->right_pair;
        }
        trail->right += value;
    } else {
        trail->left += value;
    }
}

void find_and_add_right_number(pair_t *pair, int value) {
    pair_t *trail = pair->parent;
    while (trail->right_pair == pair) {
        pair = trail;
        trail = trail->parent;
        if (!trail) {
            return;
        }
    }
    if (trail->right_pair) {
        trail = trail->right_pair;
        while (trail->left_pair) {
            trail = trail->left_pair;
        }
        trail->left += value;
    } else {
        trail->right += value;
    }
}

bool check_for_explode(pair_t *pair, int level)
{
    if (pair->left_pair) {
        if (check_for_explode(pair->left_pair, level + 1)) {
            return true;
        }
    }
    if (pair->right_pair) {
        if (check_for_explode(pair->right_pair, level + 1)) {
            return true;
        }
    }
    if (level > 4) {
        pair_t *parent = pair->parent;
        find_and_add_left_number(pair, pair->left);
        find_and_add_right_number(pair, pair->right);
        if (parent->left_pair == pair) {
            parent->left_pair = NULL;
        } else if (parent->right_pair == pair) {
            parent->right_pair = NULL;
        } else {
            abort();
        }
        free_pair(pair);
        return true;
    }
    return false;
}

void split_number(pair_t *pair, bool right)
{
    pair_t *new_pair = alloc_pair();
    new_pair->parent = pair;
    if (right) {
        new_pair->left = pair->right/2;
        new_pair->right = (pair->right + 1)/2;
        pair->right = 0;
        pair->right_pair = new_pair;
    } else {
        new_pair->left = pair->left/2;
        new_pair->right = (pair->left + 1)/2;
        pair->left = 0;
        pair->left_pair = new_pair;
    }
}

bool check_for_split(pair_t *pair)
{
    if (pair->left_pair) {
        if (check_for_split(pair->left_pair)) {
            return true;
        }
    } else if (pair->left >= 10) {
        return split_number(pair, false), true;
    }
    if (pair->right_pair) {
        if (check_for_split(pair->right_pair)) {
            return true;
        }
    } else if (pair->right >= 10) {
        return split_number(pair, true), true;
    }
    return false;
}

void reduce_pair(pair_t *pair)
{
    for (;;) {
        if (check_for_explode(pair, 1))
            continue;
        if (check_for_split(pair))
            continue;
        break;
    }
}

int calc_magnitude(pair_t *pair)
{
    int left;
    int right;
    if (pair->left_pair) {
        left = calc_magnitude(pair->left_pair);
    } else {
        left = pair->left;
    }
    if (pair->right_pair) {
        right = calc_magnitude(pair->right_pair);
    } else {
        right = pair->right;
    }
    return 3*left + 2*right;
}

int main(void)
{
    char *line;
    parse_input();
    pair_t *main_pair = NULL;

    while ((line = parse_get_line()) != NULL) {
        pair_t *pair = parse_pair(&line, NULL);
        if (!main_pair) {
            main_pair = pair;
        } else {
            main_pair = add_pair(main_pair, pair);
            reduce_pair(main_pair);
        }
    }
    printf("part 1: %d\n", calc_magnitude(main_pair));

    parse_input();
    pair_t *pairs[100];
    int idx = 0;
    int largest_magnitude = 0;

    while ((line = parse_get_line()) != NULL) {
        pairs[idx++] = parse_pair(&line, NULL);
    }
    save_pairs();
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            if (i == j) {
                continue;
            }
            restore_pairs();
            pair_t *pair = add_pair(pairs[i], pairs[j]);
            reduce_pair(pair);
            int magn = calc_magnitude(pair);
            if (magn > largest_magnitude) {
                largest_magnitude = magn;
            }
        }
    }
    printf("part 2: %d\n", largest_magnitude);
    return 0;
}
