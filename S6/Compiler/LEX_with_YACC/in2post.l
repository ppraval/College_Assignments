%{
    #include <stdio.h> 
	  #include "y.tab.h"
%}

%%

[0-9]+	{
          yylval = atoi(yytext);
          return DIGIT;
        }
"+"	  return *yytext;
"-"	  return *yytext;
[()]	  return *yytext;
[\n]      return NEWLINE;

%%

yywrap()
{
	return 1;
}