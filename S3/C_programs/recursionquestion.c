#include <stdio.h>

int jumping_down(int n, int count)
{
    printf("%d", n);
    if (n == 1)
    {
        return count;
    }
    if (n % 2 == 0)
    {
        count++;
        return jumping_down(n / 2, count);
    }
    if (n % 2 == 1)
    {
        count++;
        return jumping_down(n + 1, count);
    }
}

int main()
{
    int n = 5;
    printf("%d", jumping_down(n, 0));

    return 1;
}
