#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_STATES 100
#define MAX_ALPHABETS 26

typedef struct {
    int numStates;
    int numAlphabets;
    bool transitions[MAX_STATES][MAX_ALPHABETS][MAX_STATES];
    bool isFinal[MAX_STATES];
    int startState;
} NFA;

typedef struct {
    int numStates;
    int numAlphabets;
    bool transitions[MAX_STATES][MAX_ALPHABETS][MAX_STATES];
    bool isFinal[MAX_STATES];
    int startState;
} DFA;

int powerSet[MAX_STATES][MAX_STATES];
int numPowerSetStates = 0;

// Function to check if a set is already in the powerset
int findState(int stateSet[], int size) {
    for (int i = 0; i < numPowerSetStates; i++) {
        bool found = true;
        for (int j = 0; j < size; j++) {
            if (powerSet[i][j] != stateSet[j]) {
                found = false;
                break;
            }
        }
        if (found) {
            return i;
        }
    }
    return -1;
}

// Function to add a new state to the powerset
int addStateToPowerSet(int stateSet[], int size) {
    for (int i = 0; i < size; i++) {
        powerSet[numPowerSetStates][i] = stateSet[i];
    }
    numPowerSetStates++;
    return numPowerSetStates - 1;
}

// Function to convert NFA to DFA
void convertNFAtoDFA(NFA *nfa, DFA *dfa) {
    dfa->numAlphabets = nfa->numAlphabets;
    int queue[MAX_STATES][MAX_STATES];
    int front = 0, rear = 0;

    // Start with the start state of the NFA
    int startStateSet[MAX_STATES] = {0};
    startStateSet[0] = nfa->startState;
    int startStateSize = 1;

    queue[rear][0] = nfa->startState;
    rear++;

    addStateToPowerSet(startStateSet, startStateSize);
    dfa->startState = 0;

    while (front < rear) {
        int currentSetSize = 0;
        int currentSet[MAX_STATES] = {0};
        
        // Dequeue the current state set
        for (int i = 0; i < MAX_STATES; i++) {
            currentSet[i] = queue[front][i];
            if (currentSet[i] != -1) {
                currentSetSize++;
            }
        }
        front++;

        // Check if this state set contains an NFA final state
        bool isFinal = false;
        for (int i = 0; i < currentSetSize; i++) {
            if (nfa->isFinal[currentSet[i]]) {
                isFinal = true;
                break;
            }
        }

        int dfaStateIndex = findState(currentSet, currentSetSize);
        if (dfaStateIndex == -1) {
            dfaStateIndex = addStateToPowerSet(currentSet, currentSetSize);
        }

        dfa->isFinal[dfaStateIndex] = isFinal;

        // Iterate over each alphabet to see where the current set can go
        for (int symbol = 0; symbol < nfa->numAlphabets; symbol++) {
            int newStateSet[MAX_STATES] = {0};
            int newSetSize = 0;

            for (int i = 0; i < currentSetSize; i++) {
                int currentState = currentSet[i];

                for (int dest = 0; dest < nfa->numStates; dest++) {
                    if (nfa->transitions[currentState][symbol][dest]) {
                        // Add the destination state to the new state set
                        bool alreadyInSet = false;
                        for (int j = 0; j < newSetSize; j++) {
                            if (newStateSet[j] == dest) {
                                alreadyInSet = true;
                                break;
                            }
                        }
                        if (!alreadyInSet) {
                            newStateSet[newSetSize++] = dest;
                        }
                    }
                }
            }

            // If there's a new state set, enqueue it
            if (newSetSize > 0) {
                int newStateIndex = findState(newStateSet, newSetSize);
                if (newStateIndex == -1) {
                    newStateIndex = addStateToPowerSet(newStateSet, newSetSize);
                    for (int i = 0; i < newSetSize; i++) {
                        queue[rear][i] = newStateSet[i];
                    }
                    rear++;
                }
                dfa->transitions[dfaStateIndex][symbol][newStateIndex] = true;
            }
        }
    }

    dfa->numStates = numPowerSetStates;
}

// Helper function to print DFA details
void printDFA(DFA *dfa) {
    printf("Start state: {%d}\n", dfa->startState + 1);
    printf("Alphabets: ");
    for (int i = 0; i < dfa->numAlphabets; i++) {
        printf("%c ", 'a' + i);
    }
    printf("\nStates: ");
    for (int i = 0; i < dfa->numStates; i++) {
        printf("q%d ", i + 1);
    }
    printf("\nTransitions:\n");

    for (int state = 0; state < dfa->numStates; state++) {
        for (int symbol = 0; symbol < dfa->numAlphabets; symbol++) {
            printf("q%d %c: {", state + 1, 'a' + symbol);
            for (int dest = 0; dest < dfa->numStates; dest++) {
                if (dfa->transitions[state][symbol][dest]) {
                    printf(" q%d", dest + 1);
                }
            }
            printf(" }\n");
        }
    }

    printf("Final states: ");
    for (int i = 0; i < dfa->numStates; i++) {
        if (dfa->isFinal[i]) {
            printf("q%d ", i + 1);
        }
    }
    printf("\n");
}

int main() {
    NFA nfa;
    DFA dfa;
    int numFinalStates, numTransitions;
    char alphabets[MAX_ALPHABETS];

    // Input number of alphabets
    printf("Enter the number of alphabets: ");
    scanf("%d", &nfa.numAlphabets);

    printf("Enter the alphabets: ");
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
        nfa.transitions[fromState - 1][symbol - 'a'][toState - 1] = true; // Adjust to 0-based index
    }

    // Convert NFA to DFA
    convertNFAtoDFA(&nfa, &dfa);

    // Output the converted DFA
    printDFA(&dfa);

    return 0;
}
