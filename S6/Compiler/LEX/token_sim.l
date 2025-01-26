%{

/* Scan and return a token for identifiers of the format :
             string number
Note: strings are not case sensitive
Examples: a0 , A1 , ab2 , AB4 , aBc5
*/

#include<stdio.h>

#define ID 1  //Identifier token
#define ER 2  //Error token

%}

low    [a-z]
upp    [A-Z]
number [0-9]

%option noyywrap

%%

({low}|{upp})({low}|{upp})*({number})  return ID;

(.)*                                   return ER;

%%

int main()
{
  int token = yylex();
  if(token==ID)
    printf("Acceptable\n");
  else if(token==ER)
    printf("Unacceptable\n");
  return 1;
}