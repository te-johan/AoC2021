#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

int x1 = 253;
int x2 = 280;
int y1 = -73;
int y2 = -46;

int highest_y = 0;
int hit_count = 0;

bool check_hit(int x, int y)
{
    if (x >= x1 && x <= x2 && y >= y1 && y <= y2) {
        return true;
    }
    return false;
}

void run_steps(int x_vec, int y_vec)
{
    int x = 0;
    int y = 0;
    int x_org = x_vec;
    int y_org = y_vec;
    int y_top = 0;

    for (;;) {
        x += x_vec;
        y += y_vec;
        if (x_vec) {
            x_vec--;
        }
        y_vec--;
        if (y > y_top) {
            y_top = y;
        }
        if (check_hit(x, y)) {
            hit_count++;
            if (y_top > highest_y) {
                highest_y = y_top;
            }
            break;
        }
        if (x >= (x2 + 1)) {
            break;
        }
        if (y <= (y1 - 1)) {
            break;
        }
    }
}

/* target area: x=253..280, y=-73..-46 */
int main(void)
{
    int val = 0;
    for (int y = y1 - 1; y < 1000; y++) {
        for (int x = 0; x <= 1000; x++) {
            run_steps(x, y);
        }
    }
    printf("part 1: %d\n", highest_y);
    printf("part 2: %d\n", hit_count);
    return 0;
}
