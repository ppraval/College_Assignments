%{
  #include <stdio.h>
  int yyerror();
%}

%token DIGIT

%%

start : pair '\n'		{printf("\nComplete"); }
	;

pair: num ',' num	{ printf("pair(%d,%d)",$1,$3); }
  ;
num: DIGIT			{ $$=$1; }
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