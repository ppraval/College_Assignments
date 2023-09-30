#include<stdio.h>

int func(int * Arr, int n, int m)
{
    if (n<0)
        return m;
  else if(Arr[n]<m)
    return func(Arr,n-1,m + Arr[n]);
}
int main()
{
    int Arr[]={1,3,4,6,-7,-8,9},pos;
  pos=func(Arr,6,0);
  printf("Value returned: %d ",pos);
}