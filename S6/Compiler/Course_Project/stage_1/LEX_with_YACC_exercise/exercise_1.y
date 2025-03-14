%{
    #include<stdio.h>
    #include<stdlib.h>
%}

%union {
    char c;
}
%token CHAR NL
%left '+' '-'
%left '*' '/'
%type <c> expr

%%

start: expr NL {printf("\n");exit(0);}
     ;

expr: expr '*' expr {printf("%c", $<c>2);}
    | expr '-' expr {printf("%c", $<c>2);}
    | expr '+' expr {printf("%c", $<c>2);}
    | expr '/' expr {printf("%c", $<c>2);}
    | '(' expr ')' {}
    | CHAR {$<c>$=$<c>1;printf("%c", $<c>1);}
    ;

%%

int yyerror() {
    printf("\nInvalid Input!\n");
    return 0;
}

int main() {
    printf("Enter:");
    yyparse();
    return 0;
}
