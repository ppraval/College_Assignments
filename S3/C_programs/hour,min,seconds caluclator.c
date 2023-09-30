#include <stdio.h>
int main()
{
    int tseconds, hours, minutes, seconds;
    printf("Input the number of seconds = ");
    scanf("%d", &tseconds);
    hours = tseconds/3600;
    minutes = (tseconds%3600)/60;
    seconds = (tseconds - hours*3600) % 60;
    printf("Total time will be %d Hours %d minutes and %d seconds", hours, minutes, seconds);
    return 0;
} 