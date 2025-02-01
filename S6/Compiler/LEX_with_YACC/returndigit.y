%{
    #include <stdio.h>
	#include <stdlib.h>
    int yyerror();
%}

%token DIGIT

%%

start : expr '\n'		{printf("\nComplete");exit(1);}
	;

expr:  expr '+' expr	{printf("+ ");}
	| expr '*' expr		{printf("* ");}
	| '(' expr ')'
	| DIGIT	   			{printf("%d ",$1);}
	;

%%

int yyerror()
{
	printf("Error");
}

int main()
{
  yyparse();
  return 1;
}
