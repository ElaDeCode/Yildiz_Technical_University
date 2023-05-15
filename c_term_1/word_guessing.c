// Ali Özyalçın tarafindan yazılmıştır

#include <stdio.h>


int main()
{
    // customization section

    // char d[7]={'k','a','m','n','e','r','i'};
    // char kelimeler[][7]={"anemik","kameri","marine","minare", "makine","anemi","mekan","krema","kerim", "inmek", "imkan","imren","imran","erkan", "ekran", "mera","krem","mine","mira","name","ekim", "erik","kim","nem","nam","ani","kin","kir"};

    // int d[5] = {'a','e','r','k','s'};
    // char kelimeler[][6] = {"asker","serak","kars","kase","sera","akse","ark","kes","kas","ser","sek","kar"};

    // int d[6] = {'u','e','l','m','i','k'};
    // char kelimeler[][6] = {"ilmek","imlek","melik","kule","ilke","ekim","mil","kem","kum","mil","kel","kil","ilk","ile","lim","el","il"};

    int d[10] = {'m','p','g','e','y','a','o','r','u','b'};
    char kelimeler[][7] = {"mergup","baryum","uygar","amper","yorum","muare","opera","perma","erbap","boyar","burma","gurme","gaye","yoga","morg","puma","mayo","brom","obur","ramp","baro","mera","gam","bay","ego","pay","arp","gar","ray","org","mey","mor","oya","yem","bar","bre","ram","abu","ey","ay"};

    // int d[9] = {'n','f','c','y','l','e','d','i','z'};
    // char kelimeler[][6] = {"feyiz","fidye","defin","dicle","nicel","yelin","dizel","cizye","diyez","feci","dize","yeni","lenf","file","yine","ince","yedi","fide","diye","zil","dil","din","ney","diz","def","cin","nil","lif","yen","fil","iz","ez"};

    // int d[4] = {'a','e','r','s'};
    // char kelimeler[][5] = {"sera","ser","as","es"};
    
    // int d[8] = {'j','k','l','e','a','s','g','i'};
    // char kelimeler[][7] = {"aleksi","selika","silaj","asil","kase","jale","jile","aksi","eski","saki","kale","lake","asil","jak","jel","sel","sek","ela","ilk","kal","ile","sal","gak","asi","lig","al","ak"};


    // for mechanics
    int kelimeSayisi = sizeof(kelimeler)/sizeof(kelimeler[0]); // tahmin edilebilecek kelimelerin sayısı
    int harfSayisi = sizeof(d)/sizeof(d[0]);   // kullanılabilecek harflerin sayısı
    int puan = 0;   // oyuncunun puanını tutar
    int dogruTahminSayisi = 0; // kaç doğru tahminde bulunnuldu

    char tahmin[30]; // tahmin edilen sözcüğü tutar
    char dogruTahmin[kelimeSayisi][harfSayisi+1 ]; // eski dğru tahminleri tutar

    // states
    int tekrarlayanHarf; // tekrarlayan harf varsa 1
    int harfUygun; // hatalı harf yoksa 1
    int uygun; // kelime ile tahmin aynı ise 1
    int kullanildi = 0; // eski doğru tahminle şimdiki tahmin aynı ise 1

    // loop variables
    int i = 0;  // tahmin kaç harfli
    int j = 0;  // dış sayaç
    int k = 0;  // iç sayaç

    
    // oyun başladı amaç ve harf listesi veriliyor
    printf("Oyuna hosgeldiniz\nAmaciniz her harfi 1 defa kullanarak onceden belirlenen listedeki kelimeleri bilmek\nkullanabileceginiz harfler =");
        for (j = 0; j < harfSayisi; j++)
        {
            printf(" '%c'", d[j]);
        }
    printf("\nToplam kelime sayisi: %d\nOyundan cikmak icin 0 sayisini yazip onaylayin\n\nilk tahmin : ", kelimeSayisi);

    // ilk tahmin   (oyuncu oynamak istemiyorsa tahmin yapmadan çıkabilir)
    fgets (tahmin, 30, stdin);

    // oyun ana döngüsü (tahminin ilk karakteri 0 dan farklı ise oyun başlar)
    while (tahmin[0] != '0')
    {
        //tahminin kaç harften oluştuğunu bulur
        i = 0; //tahminin harf sayısı
        while (tahmin[i+1] != '\0')
        {
            i++;
        }
        tahmin[i] = '\0'; //son karakterden sonra null karakter eklenir

        //tekrarlanan harfleri arar
        tekrarlayanHarf = 0;   //tekrarlanan harf varsa 1 olarak ayrılır
        for (j = 0; j < (i-1); j++)
        {
            for (k = j + 1; k < i; k++)
            {
                if (tahmin[j]==tahmin[k])
                    tekrarlayanHarf = 1;
            }
        }
        if (tekrarlayanHarf) //tahminde aynı harf 1 den fazla kullanılmışsa puan kırmaz diğer 
            printf("ayni harfi sadece 1 kez kullanablirsiniz\n");

        //(tahmin harflerinini d de arar)
        j = 0;
        while (j < i)
        {
            harfUygun = 0;
            k = 0;
            while (k < harfSayisi && !harfUygun)
            {
                harfUygun = (tahmin[j] == d[k]);
                k++;
            }
            if (harfUygun == 0)
            {
                puan -= i;
                printf("harf listesinde olmayan bir harf kullandin\n-%d puan\n",i);
                j = i;
            }
            j++;
        }

        //girilen sözcük 2 harften kısa ise kelimeler dizisi ile tahmin karşılaştırılmaz
        if (i < 2)
        {
            printf("girilen sozcuk minimum 2 harften olusmalidir\n");
        }
        else if (harfUygun == 1 && tekrarlayanHarf == 0)
        {
            // daha önceden kullanıldıysa puan kırar
            j = 0; // kelime indeksi
            kullanildi = 0;
            while (j < dogruTahminSayisi && kullanildi == 0) // doğru tahminler listesindeki kelimeleri gezer
            {
                k = 0; // harf indeksi
                kullanildi = 1;
                while (k <= i && kullanildi == 1) // 
                {   
                    kullanildi = (dogruTahmin[j][k] == tahmin[k]);
                    k++;
                }
                if (kullanildi == 1) // aynı doğru tahminde bulunulduysa puan kırar ve uyarı verir
                {
                    puan -= i;
                    printf("ayni tahminde bulunmustun.\n-%d puan\n",i);
                    j = kelimeSayisi;
                }
                j++;
            }

            if (kullanildi == 0) // aynı doğru tahminde bulunulmadıysa
            {
                // kelimeler ve tahmin karşılaştırması
                j = 0; //kelime indkxi
                while (j < kelimeSayisi)   // kelimeler listesindeki kelimeleri dolaşır
                {
                    k = 0; // harf indexi
                    uygun = 1;
                    while (k <= i && uygun == 1)  // harfler arası dolaşır farklı harf varsa tahmin ile kelime aynı değildir (uygun = 0)
                    {
                        uygun = (kelimeler[j][k] == tahmin[k]);
                        k++;
                    }
                    if (uygun == 1) // tahmin kelimeler listesinde varsa puan verip tahmini doğru tahminler listesine ekler
                    {
                        puan += i;
                        printf("bir dogru tahminde buludun\n+%d puan\n",i);
                        for ( k=0; k <= i ; k++)
                            dogruTahmin[dogruTahminSayisi][k] = tahmin[k];
                        dogruTahminSayisi++;
                        j = kelimeSayisi;
                    }
                    j++;
                }
                // her kritere uyuyor ancak listede yoksa puan kırar
                if (uygun == 0)
                {
                    puan -= i;
                    printf("kelime kelimeler listesinde yok\n-%d puan\n",i);
                }
            }
        } 
        // yeni döngü için hazırlık
        printf("\nyeni tahmininin nedir : ");
        fgets (tahmin, 30, stdin);// sonraki tahmin
    }
    // oyun bitti puan gösterilir
    printf("oyundan ciktin\ntoplam puaninin: %d\noyundan cikmak icin enter tusuna basin" , puan);
    scanf("%c",tahmin[0]);
    return 0;
}