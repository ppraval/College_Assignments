%{
    #include<stdio.h>
    #include<string.h>
    #include<stdlib.h>
    char* strcat_3(char*, char*, char*);
%}

%union {
    char *str;
}

%token VAR NL
%left '+' '-'
%left '*' '/'
%type <str> expr

%%

start: expr NL {printf("%s\n", $<str>1);exit(0);}
     ;

expr: expr '*' expr {$<str>$=strcat_3("*", $<str>1, $<str>3);}
    | expr '-' expr {$<str>$=strcat_3("-", $<str>1, $<str>3);}
    | expr '+' expr {$<str>$=strcat_3("+", $<str>1, $<str>3);}
    | expr '/' expr {$<str>$=strcat_3("/", $<str>1, $<str>3);}
    | '(' expr ')' {$<str>$=$<str>2;}
    | VAR {$<str>$=$<str>1;}
    ;

%%

int yyerror() {
    printf("\nInvalid Input!\n");
    return 0;
}

char* strcat_3(char* str1, char* str2, char* str3) {
    char *new_str = (char*)malloc(strlen(str1) + strlen(str2) + strlen(str3) + 3);
    strcpy(new_str, str1);
    strcat(new_str, " ");
    strcat(new_str, str2);
    strcat(new_str, " ");
    strcat(new_str, str3);
    return new_str;
}

int main() {
    printf("Enter:");
    yyparse();
    return 0;
}
