#include<stdio.h>

float sine(float x, int n)
{
    float sum, current_value;
    sum = x;
    current_value = x;
    for(int i = 1; 2*i + 1 <= n; i++)
    {
        current_value *= (-1*x*x)/((2*i)*(2*i + 1));
        sum += current_value;
    }
    return sum;
}
 
int main()
{
    float x; 
    int n;
    scanf("%f", &x);
    scanf("%d", &n);
    x = x*(3.1415/180);
    printf("%.4f", sine(x, n));
    return 0;
}