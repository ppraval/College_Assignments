#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
//Praval Pattam
//B220057CS
int main()
{
    int id ;
    int counter = 100;
    id = fork() ;
    if ( id == 0 )
    {
        printf ("\nCHILD::PID: %d\n", getpid());
        counter = counter+200;
    }
    else
    {
        printf ( "\nPARENT::PID: %d\n", getpid());
        counter = counter +50;
    }
    printf ("\ncounter: %d, PID: %d\n", counter, getpid());
    return 0;
}

// The value of counter in the child process is 300.
// The value of counter in the parent process is 150.
// Conclusion: counter is not a shared variable between the parent and child processes. Each process has its own separate copy of counter. 
// This behavior is due to process memory protection, which ensures that each process has its own independent memory space.