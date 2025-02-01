%option noyywrap
%{
    #include "y.tab.h"
%}

digit      [0-9]
char       [a-zA-Z]

%%

(<=|>=|<|>|==|!=)         { return RELOP; }
if                        { return IF; }
stmt                      { return STMT; }
{char}({char}|{digit}|_)* { return ID; }
{digit}+                  { return NUM; }
[ \t]+                    ; 
\n                        { return NL; }
"("                       { return '('; }
")"                       { return ')'; }
"{"                       { return '{'; }
"}"                       { return '}'; }
.                         { printf("Invalid character: %s\n", yytext); }

%%