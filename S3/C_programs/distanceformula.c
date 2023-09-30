#include <stdio.h>
#include <math.h>
int main()
{
    float x1, y1;
    float x2, y2;
    printf("input first X coordinates: ");
    scanf ("%f", &x1);
    printf("input first y coordinates: ");
    scanf ("%f", &y1);
    printf("input second x coordinates: ");
    scanf ("%f", &x2);
    printf("input second y coordinates: ");
    scanf ("%f", &y2);
    printf ("the distance between the two points is %.2f", sqrt((pow((x1-x2), 2) + pow((y1-y2), 2))));
    return 0;

}