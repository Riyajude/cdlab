#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_STATES 100
#define MAX_ALPHABETS 26

int dfa[MAX_STATES][MAX_ALPHABETS];
bool finalStates[MAX_STATES];
bool distinguishable[MAX_STATES][MAX_STATES];
int newStates[MAX_STATES];
int numStates, numAlphabets, startState, numFinalStates;

void initializeTable() {
    // Initialize the distinguishable table
    for (int i = 0; i < numStates; i++) {
        for (int j = 0; j < numStates; j++) {
            if (i != j && (finalStates[i] != finalStates[j])) {
                distinguishable[i][j] = true;
            } else {
                distinguishable[i][j] = false;
            }
        }
    }
}

void markDistinguishable() {
    bool changed;
    do {
        changed = false;
        for (int i = 0; i < numStates; i++) {
            for (int j = 0; j < i; j++) {
                if (!distinguishable[i][j]) {
                    for (int symbol = 0; symbol < numAlphabets; symbol++) {
                        int iNext = dfa[i][symbol];
                        int jNext = dfa[j][symbol];
                        if (iNext != -1 && jNext != -1 && distinguishable[iNext][jNext]) {
                            distinguishable[i][j] = true;
                            changed = true;
                            break;
                        }
                    }
                }
            }
        }
    } while (changed);
}

void minimizeDFA() {
    initializeTable();
    markDistinguishable();

    int newNumStates = 0;

    // Initialize newStates array to -1
    for (int i = 0; i < numStates; i++) {
        newStates[i] = -1;
    }

    // Assign new state numbers based on equivalence classes
    for (int i = 0; i < numStates; i++) {
        if (newStates[i] == -1) {
            for (int j = i; j < numStates; j++) {
                if (!distinguishable[i][j]) {
                    newStates[j] = newNumStates;
                }
            }
            newNumStates++;
        }
    }

    // Print minimized DFA
    printf("Minimized DFA:\n");
    printf("Number of states: %d\n", newNumStates);
    printf("Start state: q%d\n", newStates[startState] + 1);
    
    printf("Final states: ");
    for (int i = 0; i < numStates; i++) {
        if (finalStates[i]) {
            printf("q%d ", newStates[i] + 1);
        }
    }
    printf("\nTransitions:\n");

    for (int i = 0; i < numStates; i++) {
        for (int symbol = 0; symbol < numAlphabets; symbol++) {
            if (dfa[i][symbol] != -1) {
                printf("q%d -- %c --> q%d\n", newStates[i] + 1, 'a' + symbol, newStates[dfa[i][symbol]] + 1);
            }
        }
    }
}

int main() {
    int numTransitions;
    
    // Input number of alphabets
    printf("Enter the number of alphabets: ");
    scanf("%d", &numAlphabets);

    // Input number of states
    printf("Enter the number of states: ");
    scanf("%d", &numStates);

    // Input start state
    printf("Enter the start state: ");
    scanf("%d", &startState);
    startState--; // Adjust to 0-based index

    // Input number of final states
    printf("Enter the number of final states: ");
    scanf("%d", &numFinalStates);

    printf("Enter the final states: ");
    for (int i = 0; i < numFinalStates; i++) {
        int state;
        scanf("%d", &state);
        finalStates[state - 1] = true; // Adjust to 0-based index
    }

    // Initialize transitions to -1 (no transition)
    for (int i = 0; i < numStates; i++) {
        for (int j = 0; j < numAlphabets; j++) {
            dfa[i][j] = -1;
        }
    }

    // Input transitions
    printf("Enter the number of transitions: ");
    scanf("%d", &numTransitions);

    printf("Enter transitions (fromState symbol toState):\n");
    for (int i = 0; i < numTransitions; i++) {
        int fromState, toState;
        char symbol;
        scanf("%d %c %d", &fromState, &symbol, &toState);
        dfa[fromState - 1][symbol - 'a'] = toState - 1; // Adjust to 0-based index
    }

    // Minimize DFA
    minimizeDFA();

    return 0;
}
