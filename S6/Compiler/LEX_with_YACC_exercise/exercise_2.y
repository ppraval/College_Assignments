%{
    #include<stdio.h>
    #include<stdlib.h>
%}

%union {
    char *str;
    char c;
}
%token VAR NL
%left '+' '-'
%left '*' '/'
%type <str> expr

%%

start: expr NL {printf("\n");exit(0);}
     ;

expr: expr '*' expr {printf("%c ", $<c>2);}
    | expr '-' expr {printf("%c ", $<c>2);}
    | expr '+' expr {printf("%c ", $<c>2);}
    | expr '/' expr {printf("%c ", $<c>2);}
    | '(' expr ')' {}
    | VAR {$<str>$=$<str>1;printf("%s ", $<str>1);}
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
