#include <stdio.h>
#include <stdbool.h>

#define MAX_STATES 100
#define MAX_ALPHABETS 26

typedef struct {
    int numStates;
    int numAlphabets;
    bool transitions[MAX_STATES][MAX_ALPHABETS][MAX_STATES];
    bool epsilonTransitions[MAX_STATES][MAX_STATES];
    bool isFinal[MAX_STATES];
    int startState;
} NFA;

// Function to calculate epsilon closure for a state
void epsilonClosure(int state, bool closure[], NFA *nfa) {
    closure[state] = true;
    for (int i = 0; i < nfa->numStates; i++) {
        if (nfa->epsilonTransitions[state][i] && !closure[i]) {
            epsilonClosure(i, closure, nfa);
        }
    }
}

// Function to convert epsilon-NFA to NFA
void convertToNFA(NFA *nfa) {
    bool closure[MAX_STATES][MAX_STATES] = {false};

    // Calculate epsilon closure for each state
    for (int i = 0; i < nfa->numStates; i++) {
        epsilonClosure(i, closure[i], nfa);
    }

    bool newTransitions[MAX_STATES][MAX_ALPHABETS][MAX_STATES] = {false};

    // Adjust transitions using epsilon closure
    for (int state = 0; state < nfa->numStates; state++) {
        for (int symbol = 0; symbol < nfa->numAlphabets - 1; symbol++) {
            for (int epsState = 0; epsState < nfa->numStates; epsState++) {
                if (closure[state][epsState]) {
                    for (int dest = 0; dest < nfa->numStates; dest++) {
                        if (nfa->transitions[epsState][symbol][dest]) {
                            for (int epsDest = 0; epsDest < nfa->numStates; epsDest++) {
                                if (closure[dest][epsDest]) {
                                    newTransitions[state][symbol][epsDest] = true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Copy the new transitions
    for (int i = 0; i < nfa->numStates; i++) {
        for (int symbol = 0; symbol < nfa->numAlphabets - 1; symbol++) {
            for (int j = 0; j < nfa->numStates; j++) {
                nfa->transitions[i][symbol][j] = newTransitions[i][symbol][j];
            }
        }
    }

    // Adjust final states
    for (int state = 0; state < nfa->numStates; state++) {
        for (int epsState = 0; epsState < nfa->numStates; epsState++) {
            if (closure[state][epsState] && nfa->isFinal[epsState]) {
                nfa->isFinal[state] = true;
            }
        }
    }
}

// Helper function to print NFA details
void printNFA(NFA *nfa) {
    printf("Start state: {%d}\n", nfa->startState + 1);
    printf("Alphabets: ");
    for (int i = 0; i < nfa->numAlphabets - 1; i++) {
        printf("%c ", 'a' + i);
    }
    printf("\nStates: ");
    for (int i = 0; i < nfa->numStates; i++) {
        printf("q%d ", i + 1);
    }
    printf("\nTransitions:\n");

    for (int state = 0; state < nfa->numStates; state++) {
        for (int symbol = 0; symbol < nfa->numAlphabets - 1; symbol++) {
            printf("q%d %c: {", state + 1, 'a' + symbol);
            for (int dest = 0; dest < nfa->numStates; dest++) {
                if (nfa->transitions[state][symbol][dest]) {
                    printf(" q%d", dest + 1);
                }
            }
            printf(" }\n");
        }
    }

    printf("Final states: ");
    for (int i = 0; i < nfa->numStates; i++) {
        if (nfa->isFinal[i]) {
            printf("q%d ", i + 1);
        }
    }
    printf("\n");
}

int main() {
    NFA nfa;
    int numFinalStates, numTransitions;
    char alphabets[MAX_ALPHABETS];

    // Input number of alphabets
    printf("Enter the number of alphabets: ");
    scanf("%d", &nfa.numAlphabets);

    printf("Enter the alphabets (e for epsilon): ");
    for (int i = 0; i < nfa.numAlphabets; i++) {
        scanf(" %c", &alphabets[i]);
    }

    // Input number of states
    printf("Enter the number of states: ");
    scanf("%d", &nfa.numStates);

    // Input start state
    printf("Enter the start state: ");
    scanf("%d", &nfa.startState);
    nfa.startState--; // Adjust to 0-based index

    // Input number of final states
    printf("Enter the number of final states: ");
    scanf("%d", &numFinalStates);

    printf("Enter the final states: ");
    for (int i = 0; i < numFinalStates; i++) {
        int state;
        scanf("%d", &state);
        nfa.isFinal[state - 1] = true; // Adjust to 0-based index
    }

    // Input transitions
    printf("Enter the number of transitions: ");
    scanf("%d", &numTransitions);

    for (int i = 0; i < numTransitions; i++) {
        int fromState, toState;
        char symbol;
        printf("Enter transition (fromState symbol toState): ");
        scanf("%d %c %d", &fromState, &symbol, &toState);

        if (symbol == 'e') {
            nfa.epsilonTransitions[fromState - 1][toState - 1] = true; // Adjust to 0-based index
        } else {
            nfa.transitions[fromState - 1][symbol - 'a'][toState - 1] = true; // Adjust to 0-based index
        }
    }

    // Convert epsilon-NFA to NFA
    convertToNFA(&nfa);

    // Output the converted NFA
    printNFA(&nfa);

    return 0;
}
