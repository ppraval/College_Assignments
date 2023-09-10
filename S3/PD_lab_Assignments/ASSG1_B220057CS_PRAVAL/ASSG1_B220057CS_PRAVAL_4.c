#include<stdio.h>
#include<string.h>

int main()
{
    int count[52] ={0}, location, max_number = 0;
    char character, max_character, string[100];
    gets(string);
    for (int i = 0; i < strlen(string); i++)
    {
        character = string[i];
        if (character >= 'a' && character <= 'z')
        {
            location = character - 'a';
        }
        else
        {
            location = character - 'A' + 26;
        }
        count[location] = count[location] + 1 + i;    
    }
    for (int i = 0; i < strlen(string); i++)
    {   
        character = string[i];
        if (character >= 'a' && character <= 'z')
        {
            location = character - 'a';
        }
        else
        {
            location = character - 'A' + 26;
        }
        if (max_number < count[location])
        {
            max_number = count[location];
            max_character = character;
        }
    }
    printf("%c", max_character);
    return 0;
}