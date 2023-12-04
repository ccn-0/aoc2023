#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>

static long size; // Total input size
static uint16_t line_len = 0;

typedef struct gear {
    uint16_t x;
    uint16_t y;
    uint16_t refnum[6]; // Number index
    uint16_t refc;      // Attached numbers count
    uint32_t value;     // Gear value
} gear_t;

typedef struct gear_array {
    gear_t *data;
    uint32_t size;
} gear_array_t;

int probe(const char *input, uint16_t x, uint16_t y, gear_array_t *gears, uint16_t refnum) {
    if (x >= (line_len - 1) || x < 0) return 0;
    if (y >= size/line_len  || y < 0) return 0;
    char tc = input[line_len*y + x];
    int ret = 0;
    // Not sure what symbol is meant to be, so check for anything 
    // that is not a dot, digit or new line..
    if (tc != '.' && tc != '\n' && !isdigit(tc)) {
        ret = 1;
    }
    else {
        ret = 0;
    }
    // Part 2 requires gear (symbol '*') probing
    if (tc == '*') {
        for (uint32_t i = 0; i < gears->size; i++) {
            gear_t *this_gear = &(gears->data[i]);
            if (this_gear->x == x && this_gear->y == y) {
                for (int j = 0; j < this_gear->refc; j++) {
                    if (this_gear->refnum[j] == refnum) {
                        // This gear already registers this number, exit search
                        goto exit;
                    }
                }
                // Register this number to this gear
                this_gear->refnum[this_gear->refc] = refnum;
                this_gear->refc += 1;
                goto exit;
            }
        }
        // Gear doesnt exist, so create it and register this number
        gear_t new_gear = {
            .x = x,
            .y = y,
            .refc = 1,
            .value = 0
        };
        new_gear.refnum[0] = refnum;
        gears->data[gears->size] = new_gear;
        gears->size += 1;
    }
exit:
    return ret;
}

int solve(const char *input, int *r1, int *r2) {
    while(input[line_len++] != '\n');
    int state = 0;
    char num[10];
    int num_i = 0;
    uint16_t refnum = 0;
    uint16_t numbers[2000];
    *r2 = 0;

    gear_array_t gears;
    gears.size = 0;
    gears.data = calloc(1000, sizeof(gear_t));

    int attached = 0;
    
    for (uint16_t y = 0; y < size/line_len; y++) {
        for (uint16_t x = 0; x < line_len; x++) {
            const char c = input[line_len*y + x];
            switch (state)
            {
            case 0:
                if (isdigit(c)) {
                    // Starting number parsing
                    state = 1;     
                    num[num_i] = c;
                }
                break;
            case 1:
                if (isdigit(c)) {
                    // Continuing number parsing
                    num[++num_i] = c;
                }
                else {
                    // Finished parsing a number, check if we hit a symbol
                    // during parsing and add it to result, and do cleanup
                    state = 0;
                    num[++num_i] = '\0';
                    uint16_t this_number = (uint16_t)atoi(num);
                    numbers[refnum] = this_number;
                    if (attached) {
                        *r1 += this_number;
                        attached = 0;
                    }
                    num_i = 0;
                    refnum += 1;
                }
                break;
            default:
                break;
            }
            
            if (isdigit(c)) {
                // Check for symbol adjecency
                if (probe(input, x+1, y+0, &gears, refnum)
                 || probe(input, x+1, y-1, &gears, refnum)
                 || probe(input, x+0, y-1, &gears, refnum)
                 || probe(input, x-1, y-1, &gears, refnum)
                 || probe(input, x-1, y+0, &gears, refnum)
                 || probe(input, x-1, y+1, &gears, refnum)
                 || probe(input, x+0, y+1, &gears, refnum)
                 || probe(input, x+1, y+1, &gears, refnum)
                ) {
                    attached = 1;
                }

            }
        }
    }

    // Go through all found gears and get result for part 2:
    for (uint16_t i = 0; i < gears.size; i++) {
        gear_t g = gears.data[i];
        if (g.refc == 2) {
            // Gear is attached to exactly 2 numbers, get values for result
            *r2 += numbers[g.refnum[0]] * numbers[g.refnum[1]];
        }
    }

    return 0;
}

int main() {
    FILE *f;
    int r1 = 0, r2 = 0;
    fopen_s(&f, "input.txt", "r");

    fseek(f, 0, SEEK_END);
    size = ftell(f) + 1;
    fseek(f, 0, SEEK_SET);
    char *input = (char *)malloc(sizeof(char) * size);

    while (fread(input, 1, size, f) != 0) {     
        solve(input, &r1, &r2);
    }

    printf("Part 1: %d\n", r1);
    printf("Part 2: %d\n", r2);
    fclose(f);
    free(input);
    return 0;
}
