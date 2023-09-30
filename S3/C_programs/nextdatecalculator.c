#include <stdio.h>
#include <stdlib.h>

int main ()
{
    int d, m ,y;
    int f = 0;
    printf ("Enter a date in the DD/MM/YYYY :");
    scanf ("%d %d %d", &d, &m, &y);
    if (y % 4 == 0 && y % 100 != 0 || y % 400 == 0)
        {
            f = 1;
        }   
    
    d += 1;

    switch (m)
    {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
                if (d <= 31)
                break;
        case 4:
        case 6:
        case 9:
        case 11:
                if (d <= 30)
                break;
        case 2:       
                if (d <= 29 && f == 1)
                break;
                else if (d <= 28)
                break;
        default:
            d = 1;
            m += 1;
            if (m > 12)
            {
                m = 1;
                y +=1;
            } 
    

    }

    printf ("The next ice cream date is %d/ %d/ %d", d, m, y);
    return 0;

}