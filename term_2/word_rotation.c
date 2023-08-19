#include <stdio.h>

#define MAX 50

int main()
{
    char tmp, word[MAX];
    int i, j;
    do
    {
        j = 0;
        printf("kelimeniz nedir\n");
        scanf(" %s", word);
        do
        {
            tmp = word[0];
            i = 0;
            do
            {
                word[i] = word[i + 1];
                i++;
            } while (word[i + 1] != '\0');

            word[i] = tmp;
            printf("%s\n", word);
            j++;

        } while (word[j] != '\0');

        printf("devam etmek icin e ye cikmak icin baska bir tusa basiniz\n");
        scanf(" %c", &tmp);

    } while (tmp == 'e' || tmp == 'E');

    return 0;
}