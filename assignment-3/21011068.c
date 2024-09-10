#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/*
@brief kullanicinin girdigi n ve m degerlerini kullanarak matrisin dinamik bir sekilde olusturuldugu ve tum elemanlarinin -1 yapildigi matristir.
@param n kullanicidan alinan matrisin satir sayisidir.
@param m kullanicidan alinan matrisin sutun sayisidir.
@return olusan matrisin adresini geri dondurur.
*/
int** matris_olustur(int n, int m) {
    int i, j;
    int **matris = (int **)malloc(n * sizeof(int *));
    if (matris == NULL) {
        printf("Bellek ayirma hatasi!\n");
        exit(EXIT_FAILURE);
    }
    
    for (i = 0; i < n; i++) {
        matris[i] = (int *)malloc(m * sizeof(int));
        if (matris[i] == NULL) {
            printf("Bellek ayirma hatasi!\n");
            exit(EXIT_FAILURE);
        }
    }
    
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            matris[i][j] = -1;
        }
    }
    return matris;
}
/*
@brief matrisi yazdirmaya yarayan fonksiyondur.
@param **matris yazdirilacak olan matrisin isaretcisidir.
@param satir matrisin satir sayisidir.
@param sutun matris sutun sayisidir.
*/
void matrisYazdir(int **matris, int satir, int sutun) {
    int i, j;
    for (i = 0; i < satir; i++) {
        for (j = 0; j < sutun; j++) {
            printf(" %d ", matris[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
/*
@brief random olarak olusturulan sayinin matriste bulunup bulunmadigini kontrol eden fonksiyondur.
@param *kapasite matrisin satirlarindaki kuyruklarin kapasitesini tutan isaretcidir.
@param n matrisin satir sayisidir.
@param **matris kontrolun yapilacagi matrisin isaretcisi
@param num rastgele olarak uretilen degerdir.
*/
int is_unique(int *kapasite, int n, int** matris, int num) {
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < kapasite[i]; j++) {// dongu her satirin kapasitesine kadar ilerler.
            if (matris[i][j] == num) {//num degeri matristeki degere esitse bu blok calismali.
                return 0;
            }
        }
    }
    return 1;//num degeri matriste yoksa return degeri 1 olur.
}
/*
@brief kullanicidan matristeki kuyruklarin kapasitesinin girisinin saglandigi ve random sayilarin matrise yerlestirildigi fonksiyondur.
@param *kapasite kullanicinin girdigi kapasitelerin saklanacagi adresin isaretcisidir.
@param **matris uzerinde islem yapilacak olann matrisin isaretcisidir.
@param n matirisin satir sayisidir.
@param m matrisin sutun sayisidir.
@return kapasite kuyruklarin kapasitesinin saklandigi dizinin isaretcisidir.
*/
int* sayiYerlestir(int *kapasite, int **matris, int n, int m) {
    int i, j, num, k;
    k = m * n * 2;
    printf("Her bir kuyrugun kapasitesini sirayla girin: ");
    for (i = 0; i < n; i++) {
        scanf("%d", &kapasite[i]);
    }
    srand(time(NULL));
    
    for (i = 0; i < n; i++) {
        for (j = 0; j < kapasite[i]; j++) {
            num = rand() % (k + 1);
            while (is_unique(kapasite, n, matris, num) == 0) {//uretilen sayi unique degilse bu blok tekrar tekrar calisir.
                num = rand() % (k + 1);
            }
            matris[i][j] = num;//unique sayi bulundugunda matrise yerlestirilir
        }
    }
    return kapasite;
}
/*
@brief max heap ozelligini saglamak icin calistirilacak fonksiyondur.
@param *dizi max hesapify isleminin gerceklestirilecegi dizidir(kuyruk yapisidir).
@param i max heapify isleminin yapilacagi alt agacin root'udur.
@param n agacin kapasitesidir.
*/

void max_heapify(int *dizi, int i, int n) {
    int left, right, largest, temp;//largest islemin yapildigi altagactaki en buyuk sayidir.
    left = 2 * i +1;//rootun solundaki elemanin indisidir.
    right = 2 * i + 2;//rootun sagindaki elemanin indisidir.
    if (left < n && dizi[left] > dizi[i]) {//sol indis degeri dizinin kapasitesini asmadiysa ve soldaki eleman root elemandan buyukse bu blok calismalidir.
        largest = left;
    }
    else {
        largest = i;
    }
    if (right < n && dizi[right] > dizi[largest]) {//sag indis degeri dizinin kapasitesini asmadiysa ve sagdaki eleman root elemandan buyukse bu blok calismalidir.
        largest = right;
    }
    if (largest != i) {//en buyuk sayi roottaki sayidan farkliysa bu blok calismalidir.
        temp = dizi[i];
        dizi[i] = dizi[largest];
        dizi[largest] = temp;
        max_heapify(dizi, largest, n);//altagac yapisinda degisik yapildigindan altagactaki tum degerlerin tekrar kontrolu saglanir.
    }
}
/*
@brief diziyi max heap yapisina donusturur.
@param *dizi max heape donusturulecek dizinin isaretcidir.
@param n dizinin kapasitesidir.
*/
void build_maxHeap(int *dizi, int n) {
    int i;
    for (i = (n-1) / 2; i >= 0; i--) {
        max_heapify(dizi, i, n);
    }
}
/*
@brief matristeki kuyruk yapilarindaki en buyuk elemanin cikarildigi ve ilgili kuyrugun tekrar max heapify ozelligi kazandirilmasina yarayan, kuyruklarda eleman kalmayana kadar bu islemin devam ettigi fonksiyondur.
@param *kapasite kuyruklarin kapasitesidir.
@param **matris islemin yapildigi matristir.
@param n matrisin satir sayisidir.
@param m matrisin sutun sayisidir.
@param toplam_kapasite matriste bulunan kuyruklarin kapasitelerinin toplamidir.
*/
void matris_islemleri(int *kapasite, int **matris, int n, int m, int toplam_kapasite) {
    int count = 0, max, max_indis, k = 0, i, j,temp;//count kuyruktan cikarilan sayi adedidir.
    int *dizi;//hangi kuyrugun tamamen bosaltildigi bilgisini tutan dizidir.
    dizi=(int*)malloc(sizeof(int)*n);
    while (count != toplam_kapasite) {//kuyruklarin tamamen bosalana kadar bu blok calisir.
        max = matris[0][0];//max deger default olarak atanir
        max_indis = 0;
        for (i = 1; i < n; i++) {// kuyruklardaki en buyuk elemanin bulunmasina yarayan kod blogudur.
            if (matris[i][0] >= max) {
                max = matris[i][0];
                max_indis = i;
            }
        }
        temp=matris[max_indis][0]; //birinci ve sonuncu elemanin yeri degistirilir.
        matris[max_indis][0]=matris[max_indis][(kapasite[max_indis])-1];
        matris[max_indis][(kapasite[max_indis])-1]=temp;
        kapasite[max_indis]--;//kapasite dusurulur
        max_heapify(matris[max_indis], 0, kapasite[max_indis]);//maxheapify yapilir.
        matris[max_indis][(kapasite[max_indis])] = -1;//son eleman -1'e esitlenir.
        
        if (kapasite[max_indis] == 0) {//kuyruk bosaldiysa diziye kuyrugun numarasi yazilir.
            dizi[k] = max_indis;
            k++;
        }
        
        count++;//kuyruklardan cikarilan eleman sayisi 1 artirilir.
        printf("Secilen sayi: %d\n", max);
        matrisYazdir(matris, n, m);
    }
    printf("Sirasiyla elemanlari tukenen kuyruklar: ");
    for (i = 0; i < n; i++) {
        printf("%d ", dizi[i]+1);
    }
    printf("\n");
}

int main() {
    int n, m, i, toplam_kapasite = 0;
    printf("n satir ve m sutunlu matris icin n ve m degerlerini girin: ");
    scanf("%d", &n);
    scanf("%d", &m);
    int **matris = matris_olustur(n, m);
    int *kapasite = (int *)malloc(n * sizeof(int));
    if (kapasite == NULL) {
        printf("Bellek ayirma hatasi!\n");
        exit(EXIT_FAILURE);
    }
    sayiYerlestir(kapasite, matris, n, m);
    printf("Rastgele olusan matris:\n");
    matrisYazdir(matris, n, m);
    for (i = 0; i < n; i++) {
        build_maxHeap(matris[i], kapasite[i]);
        toplam_kapasite += kapasite[i];
    }
    printf("\nMax heap ozelligi tasiyan kuyruklardan olusan matris:\n");
    matrisYazdir(matris, n, m);
    matris_islemleri(kapasite, matris, n, m, toplam_kapasite);
    
    
    for (i = 0; i < n; i++) {
        free(matris[i]);
    }
    free(matris);
    free(kapasite);
    
    return 0;
}


