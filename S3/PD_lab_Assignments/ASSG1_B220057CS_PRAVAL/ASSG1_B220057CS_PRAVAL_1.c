#include<stdio.h>
#include<string.h>

int main()
{
    int count;
    char character, string[100];
    fgets(string, 100, stdin);
    character = getchar();
    for(int i = 0; i < strlen(string); i++)
    {
        if ((string[i] == character) || (string[i] == character+32) || (string[i] == character-32))
        {
        count++;
        }
    }
    printf("%d", count);
    return 0; 
}