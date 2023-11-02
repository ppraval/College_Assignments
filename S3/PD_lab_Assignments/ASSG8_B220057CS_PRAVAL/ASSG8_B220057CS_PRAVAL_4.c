#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct valAndIndex
{
    int val;
    int index;
};

typedef struct valAndIndex *vi;

vi intInBrac(char arr[], int i)
{
    int val = 0;
    int j;
    vi v1 = (vi)malloc(sizeof(struct valAndIndex));
    vi vtemp;
    for(j = i + 1; i < strlen(arr); j++)
    {
        if(arr[j] != ' ' && arr[j] != '(' && arr[j] != ')')
        {
            val = (val * 10) + (arr[j] - '0');
            //printf("the arr value %c\n", arr[j]);
        }
        if(arr[j] == ')')
        {
            break;
        }
        if(arr[j] == '(')
        {
            vtemp = intInBrac(arr, j);
            j = vtemp->index;
        }
    }
    v1->index = j;
    v1->val = val;
    if(val != 0)
        printf("value is %d\n", v1->val);
    return v1;
}

int main()
{
    char string[] = "( 50 ( 3 ( 8 ( ) ( ) ) ( 4 ( ) ( ) ) ) )";
    int i = 0;
    vi v1 = intInBrac(string, i);
    return 1;
}