%{
  #include <stdio.h>
  #include <stdlib.h>
  int yyerror();
%}

%token DIGIT

%left '+'
%left '*'

%%

start: expr '\n'  		{ printf("Expression value = %d\n",$1);}
	;

expr: expr '+' expr		{$$ = $1 + $3;printf("$$ = %d, $1 = %d, $2 = %c, $3 = %d\n", $$, $1, $2, $3);}
	| expr '*' expr		{$$ = $1 * $3;printf("$$ = %d, $1 = %d, $2 = %c, $3 = %d\n", $$, $1, $2, $3);}
	| '(' expr ')'	 	{$$ = $2;printf("$$ = %d, $1 = %c, $2 = %d, $3 = %c\n", $$, $1, $2, $3);}
	| DIGIT				{$$ = $1;printf("$$ = %d, $1 = %d\n", $$, $1);}
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