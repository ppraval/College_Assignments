#include <stdio.h>
#include <stdlib.h>

int main ()
{   int size, num, previousNum, numberCheck = 0;
    printf("Please input a number\n");
    scanf("%d", &size);
    if (size <= 0 )
    {
        printf("That size doesnt exist, Please try again");
    }
    
    else
    {
        for (int  i = 1; i <= size; i++)
        {
            printf("Now please input the sequence of numbers one by one\n");
            scanf("%d", &previousNum);
            if (previousNum < 0)
            {
                printf ("Please input Positive Values");
                numberCheck = 2;
                break;
            }
            if (previousNum < num)
            {
                numberCheck = 1;
            }
            num = previousNum;

        }
        if (numberCheck == 2)
        {
            printf ("Please try again");
        }
        else
        {
        if (numberCheck == 1)
        {printf ("The series is not Very Acending");}
        else 
        {printf ("The series is Very Acending");}
        }
        return 0;
    }

}   