%{
    #include <stdio.h>
    #include <stdlib.h>
    int nesting = 0;
%}

%token RELOP ID NUM IF STMT NL

%%

start: if_stmt NL { printf("Levels of nesting are %d\n", nesting); exit(0); }
     ;

if_stmt: IF '(' cond ')' '{' stmt '}' { nesting++; }
       ;

stmt: STMT
    | if_stmt
    ;

cond: x RELOP x
    ;

x: ID
 | NUM
 ;

%%

int yyerror(const char *s) {
    printf("Syntax error: %s\n", s);
    return 0;
}

int main() {
    printf("Enter input: ");
    yyparse();
    return 0;
}