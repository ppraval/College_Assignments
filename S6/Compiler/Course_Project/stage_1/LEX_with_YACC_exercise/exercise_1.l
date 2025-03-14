%option noyywrap
%{
    #include "y.tab.h"
%}

op [*+-/()]
char [a-zA-Z]

%%

{op}    {yylval.c=yytext[0];return yytext[0];}
{char}  {yylval.c=yytext[0];return CHAR;}
\n      {return NL;}

%%