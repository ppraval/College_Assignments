%option noyywrap

%{
    #include<stdio.h>
    int lines = 0, words = 0, chars = 0; 
%}

%%

\n          {lines++; chars++;} 
[^ \t\n]+   {words++; chars += yyleng;}  
.           {chars++;}  

%%

int main(int argc, char *argv[]) 
{
    if(argc > 1) 
    {
        FILE *file = fopen(argv[1], "r");
        yyin = file;
    }

    yylex(); 

    printf("Number of lines = %d\n", lines); 
    printf("Number of words = %d\n", words); 
    printf("Number of characters = %d\n", chars); 

    if(argc > 1) 
    {
        fclose(yyin);  
    }

    return 0;
}