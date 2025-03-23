%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "y.tab.h"

    void yyerror(const char *s);
    int yylex();
    extern int yylineno;
    extern char *yytext;
    extern FILE *yyin;

    FILE *outfile;
    int temp_count = 0;

    char* newTemp() 
    {
        char* temp = (char*)malloc(10 * sizeof(char));
        sprintf(temp, "t%d", temp_count++);
        return temp;
    }

    void genCode(const char* result, const char* op, const char* arg1, const char* arg2) 
    {
        fprintf(outfile, "%s = %s %s %s\n", result, arg1, op, arg2);
    }
%}

%union 
{
    char *sval;
}

%token <sval> BCSMAIN IF ELSE WHILE INT BOOL ID NUM RELOP SEMICOLON ASSIGN LBRACE RBRACE LPAREN RPAREN ADD MULTIPLY

%type <sval> program declist decl type stmtlist stmt expr aexpr term factor

%%

program: BCSMAIN LBRACE declist stmtlist RBRACE
    {
        printf("Parsing Successful\n");
        fclose(outfile);
        exit(0);
    }
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
    {
        fprintf(outfile, "%s = %s\n", $1, $3);
    }
    | IF LPAREN expr RPAREN LBRACE stmtlist RBRACE ELSE LBRACE stmtlist RBRACE
    | WHILE LPAREN expr RPAREN LBRACE stmtlist RBRACE
;

expr: aexpr RELOP aexpr
    {
        char* temp = newTemp();
        genCode(temp, $2, $1, $3);
        $$ = temp;
    }
    | aexpr
    {
        $$ = $1;
    }
;

aexpr: aexpr ADD term
    {
        char* temp = newTemp();
        genCode(temp, "+", $1, $3);
        $$ = temp;
    }
    | term
    {
        $$ = $1;
    }
;

term: term MULTIPLY factor
    {
        char* temp = newTemp();
        genCode(temp, "*", $1, $3);
        $$ = temp;
    }
    | factor
    {
        $$ = $1;
    }
;

factor: ID
    {
        $$ = $1;
    }
    | NUM
    {
        $$ = $1;
    }
;

%%

int main(int argc, char *argv[]) 
{
    if (argc < 2) 
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(1);
    }
    FILE *file = fopen(argv[1], "r");
    if (!file) 
    {
        perror("Error opening file");
        exit(1);
    }
    yyin = file;

    outfile = fopen("output.3addr", "w");
    if (!outfile) 
    {
        perror("Error opening output file");
        exit(1);
    }

    yyparse();
    fclose(file);
    return 0;
}

void yyerror(const char *s) 
{
    printf("Syntax Error at line number %d at %s\n", yylineno, yytext);
    fclose(outfile);
    exit(1);
}