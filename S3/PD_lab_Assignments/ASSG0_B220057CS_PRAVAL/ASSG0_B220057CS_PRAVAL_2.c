#include <stdio.h>

char main()
{
	int n;
	scanf("%d", &n);
	int arr[n];
	for(int i = 0; i < n; ++i)
	{
		scanf("%d", &arr[i]);
	}
	for(int i = 1; i < n; ++i)
	{
		if (arr[0] < arr[i])
			arr[0] = arr[i];
	}
			printf("%d", arr[0]);

	
return 'a';
}
