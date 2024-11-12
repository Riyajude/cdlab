#include <stdio.h>
#include <string.h>

int n;
char prods[50][50];
char firsts[26][50];
int is_first_done[26];

char follows[26][50];
int is_follow_done[26];

int isTerminal(char c) {
    return !(c >= 'A' && c <= 'Z');
}

void add_to_set(char *set, int *index, char symbol) {
    if (!strchr(set, symbol)) {
        set[*index] = symbol;
        (*index)++;
    }
}

void first(char nonterm) {
    char curr_firsts[50] = "";
    int index = 0;
    
    for (int i = 0; i < n; i++) {
        if (prods[i][0] == nonterm) {
            int k = 2, found_epsilon = 1;
            
            while (prods[i][k] != '\0' && found_epsilon) {
                found_epsilon = 0;
                char symbol = prods[i][k];
                
                if (isTerminal(symbol)) {
                    add_to_set(curr_firsts, &index, symbol);
                    break;
                } else {
                    if (!is_first_done[symbol - 'A'])
                        first(symbol);

                    int j = 0;
                    while (firsts[symbol - 'A'][j] != '\0') {
                        if (firsts[symbol - 'A'][j] == 'e')
                            found_epsilon = 1;
                        else
                            add_to_set(curr_firsts, &index, firsts[symbol - 'A'][j]);
                        j++;
                    }
                    k++;
                }
            }
        }
    }

    curr_firsts[index] = '\0';
    strcpy(firsts[nonterm - 'A'], curr_firsts);
    is_first_done[nonterm - 'A'] = 1;
}

void follow(char nonterm) {
    char curr_follows[50] = "";
    int index = 0;

    if (nonterm == prods[0][0])
        add_to_set(curr_follows, &index, '$');

    for (int i = 0; i < n; i++) {
        int k = 2;
        while (prods[i][k] != '\0') {
            if (prods[i][k] == nonterm) {
                if (prods[i][k + 1] != '\0') {
                    char next = prods[i][k + 1];

                    if (isTerminal(next))
                        add_to_set(curr_follows, &index, next);
                    else {
                        int j = 0;
                        while (firsts[next - 'A'][j] != '\0') {
                            if (firsts[next - 'A'][j] != 'e')
                                add_to_set(curr_follows, &index, firsts[next - 'A'][j]);
                            j++;
                        }

                        if (strchr(firsts[next - 'A'], 'e') && prods[i][0] != nonterm) {
                            if (!is_follow_done[prods[i][0] - 'A'])
                                follow(prods[i][0]);
                            j = 0;
                            while (follows[prods[i][0] - 'A'][j] != '\0') {
                                add_to_set(curr_follows, &index, follows[prods[i][0] - 'A'][j]);
                                j++;
                            }
                        }
                    }
                } else if (prods[i][0] != nonterm) {
                    if (!is_follow_done[prods[i][0] - 'A'])
                        follow(prods[i][0]);

                    int j = 0;
                    while (follows[prods[i][0] - 'A'][j] != '\0') {
                        add_to_set(curr_follows, &index, follows[prods[i][0] - 'A'][j]);
                        j++;
                    }
                }
            }
            k++;
        }
    }

    curr_follows[index] = '\0';
    strcpy(follows[nonterm - 'A'], curr_follows);
    is_follow_done[nonterm - 'A'] = 1;
}

int main() {
    printf("Enter the number of productions\n");
    scanf("%d", &n);
    printf("Enter productions: \n");
    for (int i = 0; i < n; i++)
        scanf("%s", prods[i]);

    for (int i = 0; i < n; i++)
        if (!is_first_done[prods[i][0] - 'A'])
            first(prods[i][0]);

    for (int i = 0; i < n; i++)
        if (!is_follow_done[prods[i][0] - 'A'])
            follow(prods[i][0]);

    printf("Firsts:\n");
    for (int i = 0; i < 26; i++)
        if (is_first_done[i])
            printf("%c : %s\n", i + 'A', firsts[i]);

    printf("Follows:\n");
    for (int i = 0; i < 26; i++)
        if (is_follow_done[i])
            printf("%c : %s\n", i + 'A', follows[i]);
}

/* input
S=aBD
B=cC
C=bC
C=e
D=EF
D=e
E=e
E=g
F=f
F=e
*/
