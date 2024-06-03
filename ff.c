#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PROD 10
#define MAX_LEN 10

// Function to find FIRST set of a non-terminal
void find_first(char, int);

// Function to find FOLLOW set of a non-terminal
void find_follow(char, int);

char production[MAX_PROD][MAX_LEN];
char non_terminals[MAX_PROD];
int n;

int main() {
    int i;

    printf("Enter the number of productions: ");
    scanf("%d", &n);

    printf("Enter the productions (Format: A->alpha): \n");
    for (i = 0; i < n; i++) {
        scanf("%s", production[i]);
        non_terminals[i] = production[i][0];
    }

    printf("First sets: \n");
    for (i = 0; i < n; i++) {
        printf("FIRST(%c) = { ", non_terminals[i]);
        find_first(non_terminals[i], 0);
        printf("}\n");
    }

    printf("\nFollow sets: \n");
    for (i = 0; i < n; i++) {
        printf("FOLLOW(%c) = { ", non_terminals[i]);
        find_follow(non_terminals[i], 0);
        printf("}\n");
    }

    return 0;
}

void find_first(char c, int pos) {
    int i;
    char result[MAX_PROD];

    if (!isupper(c)) {
        printf("%c ", c);
        return;
    }

    for (i = 0; i < n; i++) {
        if (production[i][0] == c) {
            if (production[i][3] == '$') {
                result[pos++] = '$';
            } else if (!isupper(production[i][3])) {
                result[pos++] = production[i][3];
            } else {
                find_first(production[i][3], pos);
            }
        }
    }

    for (i = 0; i < pos; i++) {
        printf("%c ", result[i]);
    }
}

void find_follow(char c, int pos) {
    int i, j;
    char result[MAX_PROD];

    if (c == non_terminals[0]) {
        result[pos++] = '$';
    }

    for (i = 0; i < n; i++) {
        for (j = 3; j < strlen(production[i]); j++) {
            if (production[i][j] == c) {
                if (production[i][j+1] == '\0') {
                    if (production[i][0] != c) {
                        find_follow(production[i][0], pos);
                    }
                } else if (!isupper(production[i][j+1])) {
                    result[pos++] = production[i][j+1];
                } else {
                    find_first(production[i][j+1], pos);
                }
            }
        }
    }

    for (i = 0; i < pos; i++) {
        printf("%c ", result[i]);
    }
}
