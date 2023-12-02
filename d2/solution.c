#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_LINE_LENGTH 200

int solve(const char *input, int *result_part1, int *result_part2, int game) {
    int state = 0;
    char num[5];
    int num_i = 0;
    int r = 0, g = 0, b = 0;
    int rm = 0, gm = 0, bm = 0;
    int invalid = 0;
    for (int i = 0; input[i]; i++) {
        char c = input[i];
        switch(state){
            case 0:
                if (c == ':') {
                    state = 1;
                }
                break;
            case 1: // Searching for digit or semicolon
                if (isdigit(c)) {
                    state = 2;
                    num_i = 0;
                    num[num_i] = c;
                }
                else if (c == ';' || '\n') {
                    // Validate set
                    if (r > 12 || g > 13 || b > 14) {
                        invalid = 1;
                    }
                    rm = max(rm, r);
                    gm = max(gm, g);
                    bm = max(bm, b);
                    r = g = b = 0;
                }
                break;
            case 2: // Parsing digits
                if (isdigit(c)) {
                    num[++num_i] = c;
                }
                else {
                    num[++num_i] = '\0';
                    state = 3;
                }
                break;
            case 3: // parsing first letter of color
                if (c == 'r') {
                    r = atoi(num);
                }
                else if (c == 'g') {
                    g = atoi(num);
                }
                else if (c == 'b') {
                    b = atoi(num);
                }
                state = 1;
                break;
        }
    }
    if (!invalid) {
        *result_part1 += game;
    }
    *result_part2 += rm*gm*bm;
    return 0;
}

int main() {
    FILE *f;
    char line[MAX_LINE_LENGTH];
    int r1 = 0, r2 = 0;
    fopen_s(&f, "input.txt", "r");
    int game = 1;
    while (fgets(line, sizeof(line), f) != NULL) {
        solve(line, &r1, &r2, game);
        game++;
    }
    printf("Part 1: %d\n", r1);
    printf("Part 2: %d\n", r2);
    fclose(f); 
    return 0;
}
