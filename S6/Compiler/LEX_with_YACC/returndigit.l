%option noyywrap
%{
  #include "y.tab.h"
%}

number  [0-9]+

%%

{number}  { yylval = atoi(yytext); return DIGIT; }
\n        { return '\n'; }
.         { return *yytext; }

%%