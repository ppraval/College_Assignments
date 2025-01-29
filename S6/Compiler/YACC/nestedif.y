%{
#include <stdio.h>
int level = 0; // Tracks the nesting level of IF statements
%}

%token IF THEN ELSE TRUE FALSE

%%

start: statement
     ;

statement: if_statement
         | ';' 
         ;

if_statement: IF '(' condition ')' THEN statement ELSE statement 
            { 
                level++; 
                printf("Nested IF level: %d\n", level); 
                level--; 
            }
           | IF '(' condition ')' THEN statement 
            { 
                level++; 
                printf("Nested IF level: %d\n", level); 
                level--; 
            }
           ;

condition: TRUE  { printf("Condition: True\n"); }
         | FALSE { printf("Condition: False\n"); }
         ;

%%

yyerror(char *s) {
    printf("Error: %s\n", s);
}

yylex() {
    int c;
    while ((c = getchar()) == ' ' || c == '\t'); 

    if (c == EOF) return 0;

    // Check for IF
    if (c == 'i') {
        if ((c = getchar()) == 'f') return IF;
        ungetc(c, stdin);
    }
    // Check for THEN
    if (c == 't') {
        if ((c = getchar()) == 'h') {
            if ((c = getchar()) == 'e') {
                if ((c = getchar()) == 'n') return THEN;
                ungetc(c, stdin);
            }
            ungetc(c, stdin);
        }
        ungetc(c, stdin);
    }
    // Check for ELSE
    if (c == 'e') {
        if ((c = getchar()) == 'l') {
            if ((c = getchar()) == 's') {
                if ((c = getchar()) == 'e') return ELSE;
                ungetc(c, stdin);
            }
            ungetc(c, stdin);
        }
        ungetc(c, stdin);
    }
    // Check for TRUE
    if (c == 'T') {
        if ((c = getchar()) == 'r') {
            if ((c = getchar()) == 'u') {
                if ((c = getchar()) == 'e') return TRUE;
                ungetc(c, stdin);
            }
            ungetc(c, stdin);
        }
        ungetc(c, stdin);
    }
    // Check for FALSE
    if (c == 'F') {
        if ((c = getchar()) == 'a') {
            if ((c = getchar()) == 'l') {
                if ((c = getchar()) == 's') {
                    if ((c = getchar()) == 'e') return FALSE;
                    ungetc(c, stdin);
                }
                ungetc(c, stdin);
            }
            ungetc(c, stdin);
        }
        ungetc(c, stdin);
    }

    return c;
}

int main() {
    printf("Enter nested IF statements (e.g., 'IF (True) THEN IF (False) THEN ; ELSE ;'):\n");
    yyparse();
    return 0;
}