%{
 
/*
1.Request input of an even and an odd number
2.indicate input characteristic : Even/Odd
3.check for input's correctness and print result
*/
 
#include <stdlib.h>
#include <stdio.h>
 
int number_1;
int number_2;
 
%}
 
 
number_sequence [0-9]*
 
%%
 
{number_sequence}[0|2|4|6|8] {
                                printf("Even number [%d]",yyleng);
                                return atoi(yytext);
                             }
 
{number_sequence}[1|3|5|7|9] {
                                printf("Odd number [%d]",yyleng);
                                return atoi(yytext);
                              }
%%
 
int yywrap()
{
  return 1;
}
 
int main()
{
  printf("\nInput an even number and an odd number\n");
  number_1 = yylex();
  number_2 = yylex();
  int diff = number_1 - number_2;
  if(diff%2!=0)
    printf("\nYour inputs were checked for correctness, \nResult : Correct\n");
  else
    printf("\nYour inputs were checked for correctness, \nResult : Incorrect\n");
  return 1;
}