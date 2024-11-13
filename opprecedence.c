#include<stdio.h>
#include <string.h>
char stack[20],inp[20],opt[10][10],ter[10];
int i,j,k,top=0,row,col,n;
void main() {
	for (i = 0; i < 10; i++) { 
		stack[i] = NULL; 
		inp[i] = NULL; 
		for (j = 0; j < 10; j++) { 
			opt[i][j]= NULL; 
		} 
	}
	printf("Enter the no.of terminals :\n");
	scanf("%d", &n);
	printf("Enter the terminals :\n");
	scanf("%s", &ter);
	printf("Enter the table values :\n");
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
		    printf("Enter the value for %c %c:\n", ter[i], ter[j]);
		    scanf(" %c", &opt[i][j]);
		}
	}
	printf("table\n");
	for (i = 0; i < n; i++) {
		printf("\t%c",ter[i]);
	}
	for(i = 0; i < n; i++){
		printf("\n%c",ter[i]);
		for(j=0;j<n;j++){
			printf("\t%c",opt[i][j]);
		}
	}
	stack[top]='$';
	printf("string");
	scanf("%s",inp);
	i=0;
	printf("\nSTACK\t\t\tINPUT STRING\t\t\tACTION\n"); 
	printf("\n%s\t\t\t%s\t\t\t", stack, inp);
	while(i<=strlen(inp)){
		for(k=0;k<n;k++){
			if (stack[top] == ter[k]) col = k; if (inp[i] == ter[k]) row = k;
		}
		if ((stack[top] == '$') && (inp[i] == '$')) { 
			printf("String is accepted\n"); 
			break; 
		} else if(opt[col][row]=='<' || opt[col][row]=='='){
			stack[++top]=opt[col][row];
			stack[++top]=inp[i];
			printf("shift %c",inp[i]);
			i++;
		} else{
			if(opt[col][row]=='>'){
				while(stack[top]!='<'){
					--top;
				}
				top--;
				printf("reduce");
			}else{
				printf("string unacceptable\n");
			}
		}
		printf("\n");
		for (k = 0; k <= top; k++) { printf("%c", stack[k]); } printf("\t\t\t"); for (k = i; k < strlen(inp); k++) { printf("%c", inp[k]); } printf("\t\t\t"); }getchar();
	}
