%{
    #include <stdio.h>
    #include <ctype.h>
%}

%token LETTER DIGIT

%%

start: variable '\n'   { printf("Valid variable\n"); }
     ;

variable: LETTER end
        ;

end: LETTER end
    | DIGIT end
    | /* epsilon */
    ;

%%

yyerror(char *s) {
    printf("Error: %s\n", s);
}

yylex() {
    int c;
    while ((c = getchar()) == ' ' || c == '\t'); 

    if (c == EOF) return 0;

    if (isalpha(c)) {
        return LETTER;
    }
    if (isdigit(c)) {
        return DIGIT;
    }

    return c;
}

int main() {
    yyparse();
    return 0;
}