#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NMAX 50
#define PLAYER_COUNT 2

int main()
{
    srand(time(NULL));
    int matrix[NMAX][NMAX], rotated[NMAX][NMAX];
    int points_of_player[PLAYER_COUNT] = {0};
    int winner = 1;
    int n;
    int i, j, k;
    char input;
    do
    {
        printf("matrisin boyutu (n x n): ");
        scanf("%d", &n);

        /*fill random 1-N^2  and print*/
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
            {
                matrix[i][j] = rand() % (n * n) + 1;
                printf("%3d ,", matrix[i][j]);
            }
            printf("\n");
        }
        printf("\n");

        /*do it for 3 times player (3 for each)*/
        for (k = 0; k < 3 * PLAYER_COUNT; k++)
        {
            /*rotated matrix*/
            for (i = 0; i < n; i++)
            {
                for (j = 0; j < n; j++)
                {
                    /*tmp = matrix[j][n-1-i];*/
                    rotated[j][n - 1 - i] = matrix[i][j];
                }
            }

            /*apply rotation*/
            for (i = 0; i < n; i++)
            {
                for (j = 0; j < n; j++)
                {
                    matrix[i][j] = rotated[i][j];
                }
            }
            /*sum of bottom*/
            for (j = 0; j < n; j++)
            {
                points_of_player[k / 3] += matrix[n-1][j];
            }

            for (i = 0; i < n; i++)
            {
                for (j = 0; j < n; j++)
                {
                    printf("%3d ,", matrix[i][j]);
                }
                printf("\n");
            }
            printf("\n");

            for (i = 0; i < PLAYER_COUNT; i++)
            {
                printf("%d. oyuncunun puani: %d\n", i + 1, points_of_player[i]);
            }
        }

        for (i = 0; i < PLAYER_COUNT - 1; i++)
        {
            if (points_of_player[i] < points_of_player[i + 1])
                winner = i + 2;
        }
        printf("kazanan: %d. oyuncu\n", winner);

        printf("devam etmek icin e tusuna basip onaylayin: ");
        scanf(" %c", &input);
    } while (input == 'E' || input == 'e');

    return 0;
}