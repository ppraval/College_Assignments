#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<signal.h>

void primes(int start, int end)
{
    printf("Hello from %d, I have computed prime numbers from <%d, %d>, my parent pid is %d\n", getpid(), start, end, getppid());
    for(int j = start; j < end; j++)
    {
        int isPrime = 1;
        if(j < 2)
            isPrime = 0;
        for(int i = 2; i * i <= j; i++)
        {
            if(j % i == 0)
            {
                isPrime = 0;
                break;
            }
        }
        if(isPrime)
            printf("%d ", j);
    }
    printf("\n");
    exit(0);
}

void grandchild() 
{
    for(int i = 0; i < 10; i++) 
    {
        printf("Hello from %d, I have no work. My parent pid is %d\n", getpid(), getppid());
        sleep(1);
    }
    exit(0);
}

int main()
{
    int k;
    printf("Enter an integer k (> 1000000): ");
    scanf("%d", &k);
    pid_t child1, child2, grand_child1, grand_child2, grand_child3, grand_child4;

    child1 = fork();
    if(child1 == 0)
    {
        grand_child1 = fork();
        if(grand_child1 == 0)
        {
            grandchild();
        }
        grand_child2 = fork();
        if(grand_child2 == 0)
        {
            grandchild();
        }
        primes(2, k / 2);
        sleep(10000);
    }
    else
    {
        child2 = fork();
        if(child2 == 0)
        {
            grand_child3 = fork();
            if(grand_child3 == 0)
            {
                grandchild();
            }
            grand_child3 = fork();
            if(grand_child3 == 0)
            {
                grandchild();
            }
            primes(k / 2, k);
            sleep(10000);
        }
        else
        {
            int parent_status;
            // d(i): Wait for all child processes to exit
            while(wait(&parent_status) > 0);
            // d(ii): Do not wait for child processes to exit
            // printf("Original Parent process is getting executed first\n");
            // sleep(1);
        }
    }
    return 0;
}