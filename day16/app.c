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

/* keep it  simple and just store binary as a string */
char bits[6000] = { 0 };
int read_index = 0;
int total_version = 0;

int get_version(void)
{
    uint32_t version;
    char str[4] = { 0 };
    memcpy(str, &bits[read_index], 3);
    read_index += 3;
    version = strtol(str, NULL, 2);
    total_version += version;
    return version;
}

int get_type(void)
{
    char str[4] = { 0 };
    memcpy(str, &bits[read_index], 3);
    read_index += 3;
    return strtol(str, NULL, 2);
}

uint64_t get_literal(void)
{
    uint64_t value = 0;
    char str[6] = { 0 };
    do {
        memcpy(str, &bits[read_index], 5);
        read_index += 5;
        uint64_t val = strtoul(str, NULL, 2);
        value = value << 4;
        value |= val & 0xf;
    } while (*str == '1');
    return value;
}

int get_length_type_id(void)
{
    return bits[read_index++] - '0';
}

uint32_t get_11bit_num_sub_packets(void)
{
    char str[12] = { 0 };
    memcpy(str, &bits[read_index], 11);
    read_index += 11;
    return strtol(str, NULL, 2);
}

uint32_t get_15bit_total_length(void)
{
    char str[16] = { 0 };
    memcpy(str, &bits[read_index], 15);
    read_index += 15;
    return strtol(str, NULL, 2);
}

uint64_t operate_on_value(int type, int value_nr, uint64_t acc_result, uint64_t new_value)
{
    uint64_t res;
    switch (type)
    {
    case 0:
        if (value_nr == 0) {
            res = new_value;
        } else {
            res = acc_result + new_value;
        }
        break;
    
    case 1:
        if (value_nr == 0) {
            res = new_value;
        } else {
            res = acc_result * new_value;
        }
        break;
    
    case 2:
        if (value_nr == 0) {
            res = new_value;
        } else {
            res = new_value < acc_result ? new_value : acc_result;
        }
        break;
    
    case 3:
        if (value_nr == 0) {
            res = new_value;
        } else {
            res = new_value > acc_result ? new_value : acc_result;
        }
        break;
    
    case 5:
        if (value_nr == 0) {
            res = new_value;
        } else {
            res = acc_result > new_value ? 1 : 0;
        }
        break;
    
    case 6:
        if (value_nr == 0) {
            res = new_value;
        } else {
            res = acc_result < new_value ? 1 : 0;
        }
        break;
    
    case 7:
        if (value_nr == 0) {
            res = new_value;
        } else {
            res = acc_result == new_value;
        }
        break;
    
    default:
        printf("invalid type:%d\n", type);
        res = -1;
        break;
    }
    return res;
}

uint64_t get_value(void);

uint64_t handle_operator_packet(uint32_t type)
{
    uint64_t result = 0;
    int iter = 0;
    int length_type_id = get_length_type_id();
    if (length_type_id) {
        uint32_t num_sub_packets = get_11bit_num_sub_packets();
        while (num_sub_packets--) {
            result = operate_on_value(type, iter++, result, get_value());
        }
    } else {
        uint32_t total_length = get_15bit_total_length();
        uint32_t end_index = read_index + total_length;
        while (read_index != end_index) {
            result = operate_on_value(type, iter++, result, get_value());
        }
    }
    return result;
}

uint64_t get_value(void)
{
    uint32_t type;
    get_version();
    type = get_type();
    if (type == 4) {
        return get_literal();
    } else {
       return handle_operator_packet(type);
    }
}

int main(void)
{
    char *line;
    parse_input();

    /* read areas */
    line = parse_get_line();

    int index = 0;
    while (*line) {
        char str[2] = { 0 };
        *str = *line;
        uint32_t value = strtol(str, NULL, 16);
        bits[index++] = value & 0b1000 ? '1' : '0';
        bits[index++] = value & 0b0100 ? '1' : '0';
        bits[index++] = value & 0b0010 ? '1' : '0';
        bits[index++] = value & 0b0001 ? '1' : '0';
        line++;
    }

    uint64_t value = get_value();
    printf("part 1: %d\n", total_version);
    printf("part 2: %llu\n", value);
    
    return 0;
}
