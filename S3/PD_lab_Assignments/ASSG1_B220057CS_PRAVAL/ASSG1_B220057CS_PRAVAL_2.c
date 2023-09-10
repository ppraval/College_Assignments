#include <stdio.h>

int natural_nos(int n) 
{ 
    if (n == 0) 
        return 0;
    natural_nos(n - 1);
    if (n != 1)
        printf(" ");
    printf("%d", n);
}

int main() 
{ 
    int n; 
    scanf("%d", &n);
    natural_nos(n);
    return 0;
}