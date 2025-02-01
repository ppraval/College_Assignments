%{
    #include "y.tab.h" 
%}

%%

"BcsMain"   { return BCSMAIN; }
"if"        { return IF; }
"else"      { return ELSE; }
"while"     { return WHILE; }
"int"       { return INT; }
"bool"      { return BOOL; }
"+"         { return PLUS; }
"*"         { return MULTIPLY; }
"="         { return ASSIGN; }
"<"         { return LT; }
">"         { return GT; }
"<="        { return LE; }
">="        { return GE; }
"=="        { return EQ; }
"!="        { return NE; }
";"         { return SEMICOLON; }
"{"         { return LBRACE; }
"}"         { return RBRACE; }
"("         { return LPAREN; }
")"         { return RPAREN; }
[a-zA-Z][a-zA-Z0-9]* { yylval.id = strdup(yytext); return ID; }
[0-9]+      { yylval.num = atoi(yytext); return NUM; }
[ \t\n]     ; // Ignore whitespace
.           { return ERROR; } // Handle invalid characters

%%

int yywrap() 
{
    return 1;
}