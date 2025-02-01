%option noyywrap

%{
    #include <stdio.h>
%}

%%

"//"[^\n]*                      {printf("Single-line comment: %s\n", yytext);}  
"/*"([^*]|"*"+[^*/])*"*"+"/"    {printf("Multi-line comment: %s\n", yytext);}
.|\n                            ; // Ignore all other characters

%%

int main(int argc, char *argv[]) 
{
    if (argc > 1) 
    {
        FILE *file = fopen(argv[1], "r");
        if (!file) 
        {
            perror("Error opening file");
            return 1;
        }
        yyin = file;
    }

    yylex();

    if (argc > 1) 
    {
        fclose(yyin);  
    }

    return 0;
}