%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylex();
extern int yyparse();
extern FILE *yyin;

void yyerror(const char *s) {
    fprintf(stderr, "Syntax Error\n");
    exit(1);
}

%}

%token BCSMAIN IF ELSE WHILE INT BOOL
%token PLUS MULTIPLY ASSIGN LT GT LE GE EQ NE
%token SEMICOLON LBRACE RBRACE LPAREN RPAREN
%token <id> ID
%token <num> NUM

%%

program: BCSMAIN LBRACE declist stmtlist RBRACE { printf("Parsing Successful\n"); }
        ;

declist: declist decl
       | decl
       ;

decl: type ID SEMICOLON
    ;

type: INT
    | BOOL
    ;

stmtlist: stmtlist SEMICOLON stmt
        | stmt
        ;

stmt: ID ASSIGN aexpr
    | IF LPAREN expr RPAREN LBRACE stmtlist RBRACE ELSE LBRACE stmtlist RBRACE
    | WHILE LPAREN expr RPAREN LBRACE stmtlist RBRACE
    ;

expr: aexpr relop aexpr
    | aexpr
    ;

aexpr: aexpr PLUS term
     | term
     ;

term: term MULTIPLY factor
    | factor
    ;

factor: ID
      | NUM
      ;

relop: LT
     | GT
     | LE
     | GE
     | EQ
     | NE
     ;

%%

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        perror("Error opening file");
        return 1;
    }

    yyparse();

    fclose(yyin);
    return 0;
}