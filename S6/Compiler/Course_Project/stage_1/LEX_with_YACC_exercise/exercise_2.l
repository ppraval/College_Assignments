%option noyywrap
%{
    #include "y.tab.h"
%}

op [*+-/()]
var [a-zA-Z]+

%%

{op} {yylval.c=yytext[0];return yytext[0];}
{var} {yylval.str=yytext;return VAR;}
\n {return NL;}

%%