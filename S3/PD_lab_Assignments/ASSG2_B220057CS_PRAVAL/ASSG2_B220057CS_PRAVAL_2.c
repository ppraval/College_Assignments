#include <stdio.h>
#include <stdlib.h>

int canComplete(int max_time_per_day, int Time[], int N, int M) 
{
    int day = 1;
    int day_time = 0;
    for(int i=0; i < M; i++)
    {
        day_time += Time[i];
        if(day_time > max_time_per_day)
        {
            day ++;
            day_time = Time[i];
        }
        
        if(day > N || Time[i] > max_time_per_day) 
        {
            return 0;
        }
    }
    return 1;
}

int main() 
{
    int N, M;
    scanf("%d", &N);
    scanf("%d", &M);
    
    int *Time = malloc(sizeof(int) * M);
    for (int i = 0; i < M; i++)
    {
        scanf("%d", &Time[i]);
    }
    
    int total_time = 0;
    for(int i=0; i < M; i++)
        total_time += Time[i];

    int low = 0, high = total_time, mid;
    int per_day_time = total_time;
    
    while(low <= high)
    {
        mid = (low + high) / 2;
        if(canComplete(mid, Time, N, M) == 1) 
        {
            per_day_time = mid;
            high = mid - 1;
        }
        else 
        {
            low = mid + 1;
        }
    }
    
    free(Time);
    printf("%d" , per_day_time);
    return 1;
}

