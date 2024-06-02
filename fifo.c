#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **productions;
int fvar;

int findPos(char NonTer) {
    int i = 0;
    while (productions[i][0] != NonTer)
        i++;
    return i;
}

char *findGenerating(char Ter) {
    int i = 0;
    while (productions[i][0] != Ter)
        i++;
    return productions[i];
}

void findFirst(char *prod, char *firstSet) {
    int i, j = 0;
    int addedTerminal = 0; // Flag to track if a terminal has been added to firstSet
    for (i = 3; i < strlen(prod); i++) {
        if (prod[i] == '|') {
            addedTerminal = 0; // Reset the flag for each alternative
            continue; 
        } else if ((prod[i] >= 'a' && prod[i] <= 'z') || prod[i] == ')' || prod[i] == '(' || prod[i] == ',') {
            if (addedTerminal) {
                strcat(firstSet, ", ");
            }
            firstSet[j++] = prod[i];
            firstSet[j] = '\0'; // Ensure null-termination after adding each terminal
            addedTerminal = 1;
            while (prod[i] != '/' && prod[i] != '\0' && prod[i] != '|') // Continue until the end of alternative
                i++;
        } else if (prod[i] >= 'A' && prod[i] <= 'Z') {
            char tempFirstSet[20];
            findFirst(findGenerating(prod[i]), tempFirstSet);
            if (addedTerminal) {
                strcat(firstSet, ", ");
            }
            strcat(firstSet, tempFirstSet);
            addedTerminal = 1;
            // If epsilon is not in the FIRST set of this non-terminal, stop processing alternatives
            if (strstr(tempFirstSet, "#") == NULL)
                return;
        } else if (prod[i] == '#') {
            if (addedTerminal) {
                strcat(firstSet, ", ");
            }
            strcat(firstSet, "#");
            addedTerminal = 1;
        }
    }
}







void findFollow(char GeneratingSymbol, int n, char *followSet) {
    int i, j = 0;
    if (GeneratingSymbol == 'S')
        strcat(followSet, "$");
    for (j = 0; j < n; j++) {
        for (i = 3; i < strlen(productions[j]); i++) {
            if (GeneratingSymbol == productions[j][i]) {
                if ((productions[j][i + 1] >= 'a' && productions[j][i + 1] <= 'z') ||
                    productions[j][i + 1] == ')' || productions[j][i + 1] == '(' || productions[j][i + 1] == ',') {
                    if (followSet[0] != '\0')
                        strcat(followSet, ", ");
                    followSet[strlen(followSet)] = productions[j][i + 1];
                } else if (productions[j][i + 1] >= 'A' && productions[j][i + 1] <= 'Z') {
                    char tempFollowSet[20] = ""; // Initialize temporary follow set
                    findFirst(findGenerating(productions[j][i + 1]), tempFollowSet);
                    if (followSet[0] != '\0')
                        strcat(followSet, ", ");
                    strcat(followSet, tempFollowSet);
                    // Include epsilon in the FOLLOW set if applicable
                    if (strstr(tempFollowSet, "#") && productions[j][i + 1] != GeneratingSymbol) {
                        char tempFollowSet2[20] = ""; // Initialize another temporary follow set
                        findFollow(productions[j][i + 1], n, tempFollowSet2);
                        strcat(followSet, ", ");
                        strcat(followSet, tempFollowSet2);
                    }
                } else if (i + 1 == strlen(productions[j])) {
                    findFollow(productions[j][0], n, followSet);
                }
            }
        }
    }
}

int main() {
    int j, i, n;
    printf("Enter the number of productions: ");
    scanf("%d", &n);
    getchar();  // Consume newline character left in buffer

    productions = (char **)malloc(sizeof(char *) * n);
    for (i = 0; i < n; i++) {
        productions[i] = (char *)malloc(sizeof(char) * 20);
    }

    printf("Enter productions:\n");
    for (i = 0; i < n; i++) {
        fgets(productions[i], 20, stdin);
        // Remove newline character from the end of the input
        productions[i][strcspn(productions[i], "\n")] = '\0';
    }

    // First Computation
    for (i = 0; i < n; i++) {
        printf("\nFIRST(%c) = { ", productions[i][0]);
        char firstSet[100] = "";
        findFirst(productions[i], firstSet);
        printf("%s }\n", firstSet);
    }

    // Follow Computation
    for (fvar = 0; fvar < n; fvar++) {
        printf("\nFOLLOW(%c) = { ", productions[fvar][0]);
        char followSet[100] = "";
        findFollow(productions[fvar][0], n, followSet);
        printf("%s }\n", followSet);
    }

    // Free allocated memory
    for (i = 0; i < n; i++) {
        free(productions[i]);
    }
    free(productions);

    printf("\nThe End\n");
    return 0;
}
