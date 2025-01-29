%{
#include <stdio.h>
#include <string.h>

int nest_level = 0;
%}

%token IF ELSE COND OTHER

%%

program:
    /* empty */
    | program stmt
    ;

stmt:
    IF '(' COND ')' stmt
        {
            printf("IF statement at nesting level %d\n", $1);
            nest_level--;
        }
    | IF '(' COND ')' stmt ELSE stmt
        {
            printf("IF-ELSE statement at nesting level %d\n", $1);
            nest_level--;
        }
    | OTHER
    ;

%%

yyerror(char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int yylex() {
    char input[100];
    scanf("%99s", input);

    if (strcmp(input, "if") == 0) {
        nest_level++;
        yylval = nest_level;
        return IF;
    } else if (strcmp(input, "else") == 0) {
        return ELSE;
    } else if (strcmp(input, "cond") == 0) {
        return COND;
    } else if (strcmp(input, "other") == 0) {
        return OTHER;
    } else if (input[0] == '(') {
        return '(';
    } else if (input[0] == ')') {
        return ')';
    } else {
        return 0; // End of input or invalid token
    }
}

int main() {
    yyparse();
    return 0;
}