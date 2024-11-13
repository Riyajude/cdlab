#include <stdio.h>
#include <string.h>
#include <ctype.h>
char input[10];
int i, error;
void E();
void T();
void Eprime();
void Tprime();
void F();
void main() {
    i = 0;
    error = 0;
    printf("Enter an arithmetic expression: ");
    gets(input);
    E();
    if (strlen(input) == i && error == 0)
        printf("\nAccepted..!!!\n");
    else
        printf("\nRejected..!!!\n");
}
void E() {
    printf("E -> T E'\n");   // Display rule E -> T E'
    T();       
    Eprime();  
}

void Eprime() {
    if (input[i] == '+' || input[i] == '-') { 
        printf("E' -> + T E' | - T E'\n");   // Display rule E' -> + 								//T E' | - T E'
        i++;
        T();
        Eprime();
    } else {
        printf("E' -> e\n"); 
    }
}

void T() {
    printf("T -> F T'\n");   // Display rule T -> F T'
    F();       
    Tprime(); 
}
void Tprime() {
    if (input[i] == '*' || input[i] == '/') { 
        printf("T' -> * F T' | / F T'\n");   // Display rule T' -> * 								//F T' | / F T'
        i++;
        F();
        Tprime();
    } else {
        printf("T' -> e\n");   
    }
}
void F() {
    if (isalnum(input[i])) {  
        printf("F -> id\n");   // Display rule F -> id
        i++;
    } else if (input[i] == '(') { 
        printf("F -> ( E )\n");   // Display rule F -> ( E )
        i++;
        E();
        if (input[i] == ')')
            i++;
        else
            error = 1;  // Missing closing parenthesis
    } else {
        error = 1;  // Invalid character
    }
}
