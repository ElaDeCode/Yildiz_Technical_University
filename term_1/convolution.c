#include <stdio.h>

int main()
{
    int m1, n1, m2, n2; // input*(1) and output*(2) array size m => y , n => x
    int k, sum = 0;     // filter size kxk      // sum of filter
    int girdi[20][20], filtre[20][20], cikti[20][20];
    int i1, j1, i2, j2;

    // resim
    do
    {
        printf("islenecek resmin yuksekligi = ");
        scanf("%d", &m1);
        if (m1 > 20)
            printf("resmin yuksekligi 20 den buyuk olamaz");
    } while (m1 > 20);

    do
    {
        printf("islenecek resmin eni = ");
        scanf("%d", &n1);
        if (n1 > 20)
            printf("resmin eni 20 birimden buyuk olamaz");
    } while (n1 > 20);

    printf("resmi giriniz\n");
    for (i1 = 0; i1 < m1; i1++)
        for (j1 = 0; j1 < n1; j1++)
        {
            printf("%d. satirin %d. elemani = ", i1 + 1, j1 + 1);
            scanf("%d", &girdi[i1][j1]);
        }
    // image taken

    // print
    printf("\nimage\n");
    for (i1 = 0; i1 < m1; i1++)
    {
        for (j1 = 0; j1 < n1; j1++)
        {
            printf("%-3d|", girdi[i1][j1]);
        }
        printf("\n");
    }
    printf("\n");

    // filtre
    do
    {
        printf("kare filtrenin boyutu(kxk) k = ");
        scanf("%d", &k);
        if (k >= m1 || k >= n1 || !(k % 2))
            printf("filtre resimden kucuk bir tek sayi olmali olmali\n");
    } while (k >= m1 || k >= n1 || !(k % 2));

    printf("filtreyi giriniz\n");
    for (i1 = 0; i1 < k; i1++)
    {
        for (j1 = 0; j1 < k; j1++)
        {
            printf("%d. satirin %d. elemani = ", i1 + 1, j1 + 1);
            scanf("%d", &filtre[i1][j1]);
            sum += filtre[i1][j1];
        }
    }
    // filtre taken
    // print
    printf("\nfiltre\n");
    for (i1 = 0; i1 < k; i1++)
    {
        for (j1 = 0; j1 < k; j1++)
        {
            printf("%-3d|", filtre[i1][j1]);
        }
        printf("\n");
    }
    // output dizisinin boyu hesaplanıyor
    m2 = m1 - k + 1;
    n2 = n1 - k + 1;

    for (i1 = 0; i1 < m2; i1++)
    {
        for (j1 = 0; j1 < n2; j1++)
        {
            cikti[i1][j1] = 0;
            for (i2 = 0; i2 < k; i2++)
            {
                for (j2 = 0; j2 < k; j2++)
                    cikti[i1][j1] += (girdi[i1 + i2][j1 + j2] * filtre[i2][j2]);
            }
            cikti[i1][j1] /= sum;
        }
    }

    printf("\nsonuc\n");
    for (i1 = 0; i1 < m2; i1++)
    {
        for (j1 = 0; j1 < n2; j1++)
        {
            printf("%-3d|", cikti[i1][j1]);
        }
        printf("\n");
    }

    return 0;
}
