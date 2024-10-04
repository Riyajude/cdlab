#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_STATES 100
#define MAX_ALPHABETS 26

typedef struct {
    int numStates;
    int numAlphabets;
    int transitions[MAX_STATES][MAX_ALPHABETS];
    bool isFinal[MAX_STATES];
    int startState;
} DFA;

bool distinguishable[MAX_STATES][MAX_STATES];
int equivalentClass[MAX_STATES];
bool marked[MAX_STATES][MAX_STATES];

void initializeTable(DFA *dfa) {
    // Initialize distinguishable table with false
    memset(distinguishable, 0, sizeof(distinguishable));

    // Mark distinguishable pairs between final and non-final states
    for (int i = 0; i < dfa->numStates; i++) {
        for (int j = 0; j < dfa->numStates; j++) {
            if (dfa->isFinal[i] != dfa->isFinal[j]) {
                distinguishable[i][j] = true;
            }
        }
    }
}

void markDistinguishable(DFA *dfa) {
    bool changed = true;
    while (changed) {
        changed = false;

        for (int i = 0; i < dfa->numStates; i++) {
            for (int j = 0; j < i; j++) {
                if (!distinguishable[i][j]) {
                    for (int symbol = 0; symbol < dfa->numAlphabets; symbol++) {
                        int iNext = dfa->transitions[i][symbol];
                        int jNext = dfa->transitions[j][symbol];

                        if (iNext != -1 && jNext != -1 && distinguishable[iNext][jNext]) {
                            distinguishable[i][j] = true;
                            changed = true;
                            break;
                        }
                    }
                }
            }
        }
    }
}

void computeEquivalentClasses(DFA *dfa) {
    for (int i = 0; i < dfa->numStates; i++) {
        equivalentClass[i] = i;
    }

    int classCount = dfa->numStates;

    for (int i = 0; i < dfa->numStates; i++) {
        for (int j = 0; j < i; j++) {
            if (!distinguishable[i][j]) {
                equivalentClass[j] = equivalentClass[i];
            }
        }
    }
}

void minimizeDFA(DFA *dfa) {
    initializeTable(dfa);
    markDistinguishable(dfa);
    computeEquivalentClasses(dfa);

    DFA minimizedDFA;
    minimizedDFA.numAlphabets = dfa->numAlphabets;

    // Create minimized DFA by merging equivalent states
    int stateMapping[MAX_STATES] = {0};
    int newNumStates = 0;

    for (int i = 0; i < dfa->numStates; i++) {
        if (equivalentClass[i] == i) {
            stateMapping[i] = newNumStates++;
        }
        else {
            stateMapping[i] = stateMapping[equivalentClass[i]];
        }
    }

    minimizedDFA.numStates = newNumStates;
    minimizedDFA.startState = stateMapping[dfa->startState];

    for (int i = 0; i < dfa->numStates; i++) {
        int newState = stateMapping[i];
        minimizedDFA.isFinal[newState] = dfa->isFinal[i];
    }

    for (int i = 0; i < dfa->numStates; i++) {
        for (int symbol = 0; symbol < dfa->numAlphabets; symbol++) {
            int nextState = dfa->transitions[i][symbol];
            if (nextState != -1) {
                minimizedDFA.transitions[stateMapping[i]][symbol] = stateMapping[nextState];
            }
            else {
                minimizedDFA.transitions[stateMapping[i]][symbol] = -1;
            }
        }
    }

    printf("\nMinimized DFA:\n");
    printf("Number of states: %d\n", minimizedDFA.numStates);
    printf("Start state: q%d\n", minimizedDFA.startState + 1);
    
    printf("Final states: ");
    for (int i = 0; i < minimizedDFA.numStates; i++) {
        if (minimizedDFA.isFinal[i]) {
            printf("q%d ", i + 1);
        }
    }
    printf("\nTransitions:\n");
    
    for (int i = 0; i < minimizedDFA.numStates; i++) {
        for (int symbol = 0; symbol < minimizedDFA.numAlphabets; symbol++) {
            if (minimizedDFA.transitions[i][symbol] != -1) {
                printf("q%d -- %c --> q%d\n", i + 1, 'a' + symbol, minimizedDFA.transitions[i][symbol] + 1);
            }
        }
    }
}

int main() {
    DFA dfa;
    int numTransitions;

    // Input number of alphabets
    printf("Enter the number of alphabets: ");
    scanf("%d", &dfa.numAlphabets);

    // Input number of states
    printf("Enter the number of states: ");
    scanf("%d", &dfa.numStates);

    // Input start state
    printf("Enter the start state: ");
    scanf("%d", &dfa.startState);
    dfa.startState--; // Adjust to 0-based index

    // Input number of final states
    printf("Enter the number of final states: ");
    int numFinalStates;
    scanf("%d", &numFinalStates);

    printf("Enter the final states: ");
    memset(dfa.isFinal, false, sizeof(dfa.isFinal));
    for (int i = 0; i < numFinalStates; i++) {
        int state;
        scanf("%d", &state);
        dfa.isFinal[state - 1] = true; // Adjust to 0-based index
    }

    // Initialize transitions to -1 (indicating no transition)
    for (int i = 0; i < MAX_STATES; i++) {
        for (int j = 0; j < MAX_ALPHABETS; j++) {
            dfa.transitions[i][j] = -1;
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
        dfa.transitions[fromState - 1][symbol - 'a'] = toState - 1; // Adjust to 0-based index
    }

    // Minimize DFA
    minimizeDFA(&dfa);

    return 0;
}
