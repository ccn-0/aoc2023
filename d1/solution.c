#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_LINE_LENGTH 100

#define i2v(i) (i % 9 + 1)

int find_digits(const char *input, int *result_part1, int *result_part2) {
    int first_index = MAX_LINE_LENGTH;
    int first_c = 0;
    int last_index = -1;
    int last_c = 0;

    const char * words[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", \
        "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"
    };
    for (int i = 0; i < 18; i++) {
        if (i == 9) {
            // Half way in => part 1 result
            *result_part1 += 10*first_c + last_c;
        }
        const char *match = strstr(input, words[i]);     
        if (match == NULL) {
            continue;
        }
        int idx = (int)(match-input);
        if (idx < first_index) {
            first_index = idx;
            first_c = i2v(i);
        }
        // Continue searching further to find the last occurance
        const char *match2 = strstr(match+1, words[i]);
        while (match2) {
            match = match2;
            idx = (int)(match-input);
            match2 = strstr(match+1, words[i]);
        }       
        if (idx > last_index) {
            last_index = idx;
            last_c = i2v(i);
        }
    }
    *result_part2 += 10*first_c + last_c;
    
    return 0;
}

int main() {
    FILE *f;
    char line[MAX_LINE_LENGTH];
    int r1 = 0, r2 = 0;
    fopen_s(&f, "input.txt", "r");
    while (fgets(line, sizeof(line), f) != NULL) {
        find_digits(line, &r1, &r2);
    }
    printf("Part 1: %d\n", r1);
    printf("Part 2: %d\n", r2);
    fclose(f); 
    return 0;
}
