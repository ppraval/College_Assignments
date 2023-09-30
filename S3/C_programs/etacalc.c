#include <stdio.h>

int main()
{
    int distance, speeed;
    float hours;
    printf ("The distance between ur starting point and destination = ");
    scanf("%d", &distance);
    printf("The speed at which you are going at in kmph = ");
    scanf("%d", &speeed);
    hours = distance/speeed;
    float speedinmins = speeed/60.0;
    float remainingmins = (distance%speeed)/speedinmins;
    printf("The ETA should be %.2f Hours and %.2f Minutes", hours, remainingmins);
    return 0;
}