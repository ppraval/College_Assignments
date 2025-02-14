%{
    #include <stdio.h>
    #include <stdlib.h>
    void yyerror(const char *s);
    int yylex();
    extern int yylineno;
    extern char *yytext;
    extern FILE *yyin;
%}

%token BCSMAIN IF ELSE WHILE INT BOOL ID NUM RELOP SEMICOLON ASSIGN LBRACE RBRACE LPAREN RPAREN ADD MULTIPLY

%%

program: BCSMAIN LBRACE declist stmtlist RBRACE
    {
        printf("Parsing Successful\n");
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
    | IF LPAREN expr RPAREN LBRACE stmtlist RBRACE ELSE LBRACE stmtlist RBRACE
    | WHILE LPAREN expr RPAREN LBRACE stmtlist RBRACE
;

expr: aexpr RELOP aexpr
    | aexpr
;

aexpr: aexpr ADD term
    | term
;

term: term MULTIPLY factor
    | factor
;

factor: ID
    | NUM
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
    yyparse();
    fclose(file);
    return 0;
}

void yyerror(const char *s) 
{
    printf(" %d %s Syntax Error\n",yylineno,yytext);
    exit(1);
}