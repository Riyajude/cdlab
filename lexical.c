#include <stdio.h>
#include <string.h>

char line[100];

int is_operator(char c)
{
    switch (c)
    {
        case '+':   
        case '-':   
        case '*':   
        case '/':   
        case '=':   
            printf("%c - Operator\n", c);
            return 1;
    }

    return 0;
}

int is_delimiter(char c)
{
    switch (c)
    {
        case '{':
        case '}':
        case '(':
        case ')':
        case '[':
        case ']':
        case ',':
        case ';':
            printf("%c - Delimiter\n", c);
            return 1;
    }

    return 0;
}

int is_keyword(char buffer[]){
	char keywords[32][10] = {"auto","break","case","char","const","continue","default",
							"do","double","else","enum","extern","float","for","goto",
							"if","int","long","register","return","short","signed",
							"sizeof","static","struct","switch","typedef","union",
							"unsigned","void","volatile","while"};
	int i;
	for(i = 0; i < 32; ++i){
		if(strcmp(keywords[i], buffer) == 0){
			return 1;
		}
	}
return 0;}

void main()
{
    char c;
    FILE *f = fopen("input.txt", "r");

    while (fgets(line, sizeof(line), f))
    {
        // Single line commment '//', skip processing it
        int flag1 = 0;
        for (int i = 0; i < strlen(line); i++)
        {
            if (line[i] == '/' && line[i + 1] == '/')
            {
                flag1 = 1;
                break;
            }       
        }
        if (flag1)
            continue;        

        // Multi-line comment '/**/'
        int flag2 = 0;
        for (int i = 0; i < strlen(line); i++)
        {
            if (line[i] == '/' && line[i + 1] == '*')
            {
                // Skip all lines until '*/' has occured
                while (fgets(line, sizeof(line), f))
                {
                    for (int j = 0; j < strlen(line); j++)
                    {
                        if (line[j] == '*' && line[j + 1] == '/')
                            flag2 = 1;
                    }

                    if (flag2)
                        break;
                }
            }
        }
        if (flag2)
            continue;

        printf("\n%s\n", line);

        char token[100];
        int index = 0;
        strcpy(token, "");

        for (int i = 0; i < strlen(line); i++)
        {
            if (is_operator(line[i]) || is_delimiter(line[i]) || line[i] == ' ' || line[i] == '\t' || line[i] == '\n')
            {
                // Check if the token is an identifier or a keyword
                if (strcmp(token, "") != 0)
                {
                    if (is_keyword(token))
                        printf("%s - Keyword\n", token);
                    else  
                        printf("%s - Identifier\n", token);

                    strcpy(token, "");
                    index = 0;
                }
            }
            else  
            {
                token[index++] = line[i];
                token[index] = '\0';
            }
        }
    }

    fclose(f);
}
/*OUTPUT
 gcc 1_lexical_analyser.c 
hp@hp-HP-Pavilion-Laptop-14-dv2xxx:~/Downloads$ ./a.out

void main()

void - Keyword
( - Delimiter
main - Identifier
) - Delimiter

{

{ - Delimiter

    int c=22;

int - Keyword
= - Operator
c - Identifier
; - Delimiter
22 - Identifier

    int a=15;

int - Keyword
= - Operator
a - Identifier
; - Delimiter
15 - Identifier

    int b = 21;

int - Keyword
b - Identifier
= - Operator
; - Delimiter
21 - Identifier

    float c = 3.1415;

float - Keyword
c - Identifier
= - Operator
; - Delimiter
3.1415 - Identifier

    double d = 2;

double - Keyword
d - Identifier
= - Operator
; - Delimiter
2 - Identifier

    int a[4] = {1, 2, 3, 4};

int - Keyword
[ - Delimiter
a - Identifier
] - Delimiter
4 - Identifier
= - Operator
{ - Delimiter
, - Delimiter
1 - Identifier
, - Delimiter
2 - Identifier
, - Delimiter
3 - Identifier
} - Delimiter
4 - Identifier
; - Delimiter

}
} - Delimiter
*/
