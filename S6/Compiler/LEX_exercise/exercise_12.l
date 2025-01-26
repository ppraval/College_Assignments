%option noyywrap

%{
    #include <stdio.h>
    int integers = 0, floats = 0;
%}

integer     [0-9]+
float       [0-9]+\.[0-9]+

%%

{integer}   {integers++;}
{float}     {floats++;}

%%

int main(int argc, char *argv[]) 
{
    if (argc > 1) 
    {
        FILE *file = fopen(argv[1], "r");
        yyin = file;
    }

    yylex();

    printf("Number of integers = %d\n", integers);
    printf("Number of floating-point numbers = %d\n", floats);

    if (argc > 1) 
    {
        fclose(yyin);  
    }

    return 0;
}