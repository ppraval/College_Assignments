#include <stdio.h>
#include <unistd.h>

int main()
{ 
    int i;
    for(i=0; i<5; i++)
    if (fork())
    {
        fork();
    }
    printf("I love her\n");
    return 0;
}
