#include <stdio.h>
#include <unistd.h>
//Praval Pattam
//B220057CS
int main()
{ 
    int i;
    for(i=0; i<5; i++)
    if (fork())
    {
        fork();
    }
    printf("Process\n");
    return 0;
}