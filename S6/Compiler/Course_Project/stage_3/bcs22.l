%{
    #include "y.tab.h"
    int line_num = 1;
%}

%option yylineno

letter          [a-zA-Z]
digit           [0-9]
id              {letter}({letter}|{digit})*
num             {digit}+
relop           "<"|">"|"<="|">="|"=="|"!="

%%

"BcsMain"   { return BCSMAIN; }
"if"        { return IF; }
"else"      { return ELSE; }
"while"     { return WHILE; }
"int"       { return INT; }
"bool"      { return BOOL; }
{id}        { yylval.sval = strdup(yytext); return ID; }
{num}       { yylval.sval = strdup(yytext); return NUM; }
{relop}     { yylval.sval = strdup(yytext); return RELOP; }
[ \t]+      ;
[\n]        { line_num++; }
";"         { return SEMICOLON; }
"="         { return ASSIGN; }
"{"         { return LBRACE; }
"}"         { return RBRACE; }
"("         { return LPAREN; }
")"         { return RPAREN; }
"+"         { return ADD; }
"*"         { return MULTIPLY; }

%%

int yywrap() 
{
    return 1;
}