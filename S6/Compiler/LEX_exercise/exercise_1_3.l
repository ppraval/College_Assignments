%option noyywrap

%{
    #include <stdio.h>
    #include <ctype.h>
%}

word [A-Za-z]+

%%

[Aa][A-Za-z]{2} {
    for (int i = 0; i < yyleng; i++) {
        yytext[i] = toupper(yytext[i]);
    }
    printf("%s\n", yytext);
}

%%

int main() 
{
    yylex();
    return 0;
}