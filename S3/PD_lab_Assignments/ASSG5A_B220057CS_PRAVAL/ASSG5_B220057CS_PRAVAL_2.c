#include<stdio.h>

int swap(int a, int b)
{
	int temp = a;
	a = b;
	b = temp;
    return 0;
}

int heapify(int arr[], int n, int i)
{
	int largest = i;
	int left = 2 * i + 1;
	int right = 2 * i + 2;
    int temp;

	if (left < n && arr[left] > arr[largest])
    {
		largest = left;
    }
    if (right < n && arr[right] > arr[largest])
	{
    	largest = right;
    }
    if (largest != i) 
    {
        temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;
		heapify(arr, n, largest);
	}
    return 0;
}

int heapSort(int arr[], int n)
{
    int temp;
	for (int i = n / 2 - 1; i >= 0; i--)
	{
    	heapify(arr, n, i);
    }

	for (int i = n - 1; i >= 0; i--) 
    {
		temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;

		heapify(arr, i, 0);
	}

    return 0;
}


int main()
{
	int n;
    scanf("%d", &n);
    int arr[100000];
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &arr[i]);
    }

    heapSort(arr, n);
    
    for (int i = 0; i < n; i++)
    {
        if (i == 0)
        {
            printf("%d", arr[i]);
        }
        else
        {
            printf(" %d", arr[i]);
        }
    }
    return 1;
}