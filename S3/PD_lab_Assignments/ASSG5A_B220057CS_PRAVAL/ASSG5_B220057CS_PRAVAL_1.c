#include<stdio.h>
#include<stdlib.h>


int merge(int arr[], int p, int q, int r)
{
	int i, j, k;
	int n1 = q - p + 1;
	int n2 = r - q;
	int L[n1], R[n2];

	for (i = 0; i < n1; i++)
    {
		L[i] = arr[p + i];
    }
    for (j = 0; j < n2; j++)
    {
		R[j] = arr[q + 1 + j];
    }
	i = 0;
	j = 0;
	k = p;
	while (i < n1 && j < n2) 
    {
		if (L[i] <= R[j]) 
        {
			arr[k] = L[i];
			i++;
		}
		else 
        {
			arr[k] = R[j];
			j++;
		}
		k++;
	}

	while (i < n1) 
    {
		arr[k] = L[i];
		i++;
		k++;
	}

	while (j < n2) 
    {
		arr[k] = R[j];
		j++;
		k++;
	}
    return 0;
}

int mergeSort(int arr[], int p, int r)
{
	if (p < r) 
    {
		int q = p + (r - p) / 2;

		mergeSort(arr, p, q);
		mergeSort(arr, q + 1, r);

		merge(arr, p, q, r);
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

	mergeSort(arr, 0, n - 1);
    
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
