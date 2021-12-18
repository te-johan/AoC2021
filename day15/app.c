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

struct area {
    uint32_t risk;
    uint32_t total_risk; // total risk of the path. not including self.
    uint16_t x;
    uint16_t y;
    uint16_t from_x;
    uint16_t from_y;
    bool in_queue;
    struct area *q_next;
};

#define SIZE 500

struct area areas[SIZE][SIZE] = { 0 };

struct area *queue;

void enqueue_area(struct area *area) 
{
    struct area *al = queue;

    if (area->in_queue) {
        return;
    }
    if (al == NULL) {
        queue = area;
        return;
    } else {
        while (al->q_next) {
            al = al->q_next;
        }
        area->in_queue = true;
        al->q_next = area;
    }
}

struct area * dequeue_area(void)
{
    struct area *area = queue;
    if (area) {
        queue = area->q_next;
        area->q_next = NULL;
        area->in_queue = false;
        return area;
    }
    return NULL;
}

void enqueue_neighbors(struct area *area, int size)
{
    int x = area->x;
    int y = area->y;
    if (x > 0) {
        enqueue_area(&areas[x-1][y]);
    }
    if (y > 0 ) {
        enqueue_area(&areas[x][y-1]);
    }
    if (x < (size - 1)) {
        enqueue_area(&areas[x+1][y]);
    }
    if (y < (size - 1)) {
        enqueue_area(&areas[x][y+1]);
    }
}

/* check all neighbors and return the one with lowest risk*/
struct area * check_neighbors_risk(struct area * area, int size)
{
    struct area * lowest = NULL;
    int x = area->x;
    int y = area->y;
    if (x > 0) {
        if (areas[x-1][y].total_risk < (area->total_risk - area->risk)) {
            lowest = &areas[x-1][y];
        }
    }
    if (y > 0 ) {
        if (areas[x][y-1].total_risk < (area->total_risk - area->risk)) {
            if (!lowest || (lowest && lowest->total_risk > areas[x][y-1].total_risk)) {
                lowest = &areas[x][y-1];
            }
        }
    }
    if (x < (size - 1)) {
        if (areas[x+1][y].total_risk < (area->total_risk - area->risk)) {
            if (!lowest || (lowest && lowest->total_risk > areas[x+1][y].total_risk)) {
                lowest = &areas[x+1][y];
            }
        }
    }
    if (y < (size - 1)) {
        if (areas[x][y+1].total_risk < (area->total_risk - area->risk)) {
            if (!lowest || (lowest && lowest->total_risk > areas[x][y+1].total_risk)) {
                lowest = &areas[x][y+1];
            }
        }
    }
    return lowest;
}

void loop_queue(struct area *start, int size) 
{
    enqueue_area(start);
    for (;;) {
        struct area *area = dequeue_area();
        if (area == NULL) {
            printf("dequeue returned NULL\n");
            break;
        } else if (area == start) {
            area->total_risk = 0;
        } else {
            struct area *neighbor = check_neighbors_risk(area, size);
            if (neighbor == NULL) {
                /* no neighbor with lower risk. */
                continue;
            }
            area->from_x = neighbor->x;
            area->from_y = neighbor->y;
            area->total_risk = neighbor->total_risk + area->risk;

        }
        enqueue_neighbors(area, size);
    }
}

int main(void)
{
    char *line;
    parse_input();
    int x = 0;
    int y = 0;
    /* read areas */
    while ((line = parse_get_line()) != NULL) {
        while (*line) {
            areas[x][y].risk = *line - '0';
            line++;
            x++;
        }
        x = 0;
        y++;
    }
    for (x = 0; x < SIZE; x++) {
        for (y = 0; y < SIZE; y++) {
            areas[x][y].total_risk = INT32_MAX;
            areas[x][y].x = x;
            areas[x][y].y = y;
            if (x >= 100) {
                areas[x][y].risk = areas[x - 100][y].risk == 9 ? 1 : areas[x - 100][y].risk + 1;
            } else if (y >= 100) {
                areas[x][y].risk = areas[x][y - 100].risk == 9 ? 1 : areas[x][y - 100].risk + 1;
            }
        }
    }

    loop_queue(&areas[0][0], 100);
    printf("part 1: total_risk:%d\n", areas[99][99].total_risk);
    for (x = 0; x < SIZE; x++) {
        for (y = 0; y < SIZE; y++) {
            areas[x][y].total_risk = INT32_MAX;
        }
    }
    loop_queue(&areas[0][0], 500);
    printf("part 2: total_risk:%d\n", areas[499][499].total_risk);

    return 0;
}
