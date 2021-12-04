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
static uint8_t board[200][5][5] = { 0 };
static bool marking[200][5][5] = { 0 };
static int total_board = 0;
static bool won_boards[200] = { 0 };

static void set_number(uint32_t number) {
    for (int b = 0; b < total_board; b++) {
        for (int x = 0; x < 5; x++) {
            for (int y = 0; y < 5; y++) {
                if (board[b][x][y] == number) {
                    marking[b][x][y] = true;
                }
            }
        }
    }
}


static void calc_winner(uint32_t b, uint32_t new_num) {
    if (won_boards[b])
        return;
    won_boards[b] = true;
    uint32_t sum = 0;
    for (int x = 0; x < 5; x++) {
        for (int y = 0; y < 5; y++) {
            if (marking[b][x][y] == false) {
                sum += board[b][x][y];
            }
        }
    }
    printf("board: %u won res:%u\n", b, new_num * sum);
}

static int check_winner(uint32_t new_num) {
    int cnt;
    int res = -1;
    for (int b = 0; b < total_board; b++) {
        for (int x = 0; x < 5; x++) {
            cnt = 0;
            for (int y = 0; y < 5; y++) {
                if (marking[b][x][y]) {
                    cnt++;
                }
            }
            if (cnt == 5) {
                calc_winner(b, new_num);
                res = b;
            }
        }
        for (int y = 0; y < 5; y++) {
            cnt = 0;
            for (int x = 0; x < 5; x++) {
                if (marking[b][x][y]) {
                    cnt++;
                }
            }
            if (cnt == 5) {
                calc_winner(b, new_num);
                res = b;
            }
        }
    }
    return res;
}


void solution1(void) {
    char *numbers;
    char *line;


    parse_input();
    
    numbers = parse_get_line();
    parse_get_line(); // empty line

    /* load board data */
    for (;;) {
        for (int i = 0; i < 5; i++) {
            line = parse_get_line();
            board[total_board][i][0] = strtol(line, &line, 10);
            board[total_board][i][1] = strtol(line, &line, 10);
            board[total_board][i][2] = strtol(line, &line, 10);
            board[total_board][i][3] = strtol(line, &line, 10);
            board[total_board][i][4] = strtol(line, &line, 10);
        }
        total_board++;
        if (parse_get_line() == NULL) {
            break;
        }
    }

    for (;;) {
        uint32_t num = atoi(numbers);
        int winner;
        while (*numbers != '\0' && *numbers++ != ',');

        set_number(num);
        winner = check_winner(num);
        if (winner != -1) {
            break;
        }

        if (*numbers == '\0')
            break;
    }
    printf("########################\n");
}

void solution2(void) {
    char *numbers;
    char *line;
    int last_winner;
    int last_number;

    parse_input();
    
    numbers = parse_get_line();

    for (;;) {
        uint32_t num = atoi(numbers);
        int winner;
        while (*numbers != '\0' && *numbers++ != ',');

        set_number(num);
        winner = check_winner(num);
        if (winner != -1) {
            last_winner = winner;
            last_number = num;
        }

        if (*numbers == '\0')
            break;
    }
    //calc_winner(last_winner, last_number);
}

int main(const char *argv[], int argc)
{
    solution1();
    solution2();
}
