#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100
#define MAX_NODES 10
#define MAX_KENAR 9 
#define MAX_DERINLIK 100 

typedef struct{
    int agirlik;
    int hedef; 
} Kenar;

typedef struct{
    char isim;
    Kenar kenarlar[MAX_KENAR];
    int kenarSayisi;
} Node;

typedef struct{
    int nodes[MAX_NODES];
    int uzunluk;
    int cevre;
} Sekil; 


Node* dosyadanOku(char* fileName);
int nodeKontrol(char isim, Node* graph);
void nodeEkle(Node* graph, char isim);
void dfs(Node* graph,int baslangic,int node,int derinlik, 
    int pathWeight,Sekil* sekiller,int* sekilSayisi,
    int oncekiNode, int* kontrol,int* path,int pathLength);
int sekilKontrol(Sekil* sekiller,int sekilSayisi, int* path, int pathLength);
void sekilEkle(Sekil* sekiller, int* sekilSayisi, int* path, int pathLength, int pathWeight);
void sekilleriYazdir(Node* graph,Sekil* sekiller, int sekilSayisi);


int main(void){
    Node* graph=dosyadanOku("Sample4.txt");
    Sekil sekiller[MAX_SIZE]={0};
    int sekilSayisi=0,i;
    int kontrol[MAX_NODES]={0};
    int path[MAX_NODES]={0};
    int pathWeight[MAX_NODES]={0};
	
    for ( i=0; i<MAX_NODES; i++){
        dfs(graph, i, i, 0, 0, sekiller, &sekilSayisi, -1, kontrol, path, 0);
        kontrol[i]=1;
    }
    printf("Sekil sayisi: %d\n",sekilSayisi);
    sekilleriYazdir(graph,sekiller, sekilSayisi);
    return 0;
}


/**
 * @brief Bir seklin onceden mevcut olup olmadigini kontrol eder.
 * 
 * @param sekiller Bulunan sekilleri saklayan dizi.
 * @param sekilSayisi Bulunan sekillerin sayisi.
 * @param path Mevcut yoldaki dugumler.
 * @param pathLength Mevcut yolun kenar sayisi.
 * @return sekil zaten mevcutsa 1, degilse 0.
 */

int sekilKontrol(Sekil* sekiller,int sekilSayisi, int* path, int pathLength){
    int i,j,k;
	for (i = 0; i < sekilSayisi; i++){
        if (sekiller[i].uzunluk +1 == pathLength){
            int count = 0;
            for ( j=0;j<pathLength; j++){
                for ( k=0; k < pathLength; k++){
                    if (sekiller[i].nodes[j] == path[k]){
                        count++; break;
                    }
                }
            }
            if (count == pathLength) {
                return 1;
            } 
        }
    }
    return 0;    
}
/*
  @brief Dosyadan graf bilgilerini okur.
  
  @param fileName dosya adi
  @return okunan grafin adresi
 */

Node* dosyadanOku(char* fileName){
    FILE *file = fopen(fileName, "r");
    if (file == NULL){
        printf("Error: File not found\n");
        return NULL;
    }
    Node* graph = (Node*)calloc(MAX_NODES , sizeof(Node));
    if ( graph == NULL ){
        printf("HATA! Bellek ayrılamadı\n");
        return NULL;
    }

    char kaynak, hedef;
    int agirlik;

    while (fscanf(file,"%c %c %d\n", &kaynak, &hedef, &agirlik) != EOF){
        int kaynakIndex = nodeKontrol(kaynak, graph);
        if (kaynakIndex == -1){
            nodeEkle(graph, kaynak);
            kaynakIndex = nodeKontrol(kaynak, graph);
        }
        int hedefIndex = nodeKontrol(hedef, graph);
        if (hedefIndex == -1){
            nodeEkle(graph, hedef);
            hedefIndex = nodeKontrol(hedef, graph);
        }
        graph[kaynakIndex].kenarlar[graph[kaynakIndex].kenarSayisi].hedef = hedefIndex;
        graph[kaynakIndex].kenarlar[graph[kaynakIndex].kenarSayisi].agirlik = agirlik;
        graph[kaynakIndex].kenarSayisi += 1;
        graph[hedefIndex].kenarlar[graph[hedefIndex].kenarSayisi].hedef = kaynakIndex;
        graph[hedefIndex].kenarlar[graph[hedefIndex].kenarSayisi].agirlik = agirlik;
        graph[hedefIndex].kenarSayisi += 1;
    }
    

    fclose(file); 
    return graph;
}
/*
@brief Belirli bir isme sahip dugumun grafta olup olmadigini kontrol eder.
 
@param isim Kontrol edilecek dugumun ismi.
@param kontrol edilecek grafin isaretcisidir.
@return Dugumun eger mevcutsa indeksi , degilse -1 .
 */

int nodeKontrol(char isim, Node* graph){
    int i;
	for (i=0;i<MAX_NODES;i++){
        if (graph[i].isim==isim){
            return i;
        }
    }
    return -1;
}
/*
@brief Belirli bir isme sahip dugumu grafa ekler.
  
@param eklenecek grafin adresi
@param isim Eklenecek dugumun ismi.
 */
void nodeEkle(Node* graph, char isim){
    int i;
	int vertexIndex=nodeKontrol(isim, graph);
    if (vertexIndex == -1){
        for (i=0; i<MAX_NODES; i++){
            if (graph[i].isim == '\0'){
                graph[i].isim = isim;
                break;
            }
        }
    }
}
/*
  @brief dfs ile sekil bulma fonksiyonudur.
  
  @param graph sekil bulunacak grafin adresidir.
  @param baslangic DFS icin baslangic dugumu.
  @param node dfs'deki mevcut dugum.
  @param derinlik dfs'deki su anki derinlik.
  @param pathWeight dfs'deki mevcut yol agirligi.
  @param sekiller Bulunan sekillerin saklandigi dizi.
  @param sekilSayisi Bulunan sekillerin sayisini saklayan isaretci.
  @param oncekiNode dfs'deki bir onceki dugum.
  @param kontrol Ziyaret edilen dugumleri saklayan eden dizi.
  @param path bulunan yolu saklayan dizi.
  @param pathLength DFS'deki mevcut kenar sayisi.
 */

void dfs(Node* graph,int baslangic,int node,int derinlik, 
    int pathWeight,Sekil* sekiller,int* sekilSayisi,
    int oncekiNode, int* kontrol,int* path,int pathLength){
    int j;
    path[pathLength] = node;
    pathLength++;

    if (derinlik>=3 && node==baslangic){
        if (!sekilKontrol(sekiller,*sekilSayisi, path, pathLength)){
            Sekil polygon;
            polygon.uzunluk=pathLength -1;
            polygon.cevre=pathWeight;
            for ( j=0;j<pathLength;j++){
                polygon.nodes[j]=path[j];
            }
            sekiller[*sekilSayisi]=polygon;
            *sekilSayisi += 1;
        }
        return;
    }
    kontrol[node] = 1;
    int i;
    for (i=0;i<graph[node].kenarSayisi;i++){
        int next=graph[node].kenarlar[i].hedef;
        int weight=graph[node].kenarlar[i].agirlik;
        if (next!=oncekiNode){
            if (kontrol[next] && next==baslangic && derinlik>=2){
                dfs(graph, baslangic, next, derinlik+1, pathWeight+weight, sekiller, sekilSayisi, node, kontrol, path, pathLength);
            }else if (!kontrol[next]){
                dfs(graph, baslangic, next, derinlik+1, pathWeight+weight, sekiller, sekilSayisi, node, kontrol, path, pathLength);
            }
        }
    }

    kontrol[node] = 0;
    pathLength--;
}
/*
  @brief Bulunan sekilleri yazdirir.
  
  @param graph bulunan sekli saklayan yapi.
  @param sekiller Bulunan sekillerin dizisi.
  @param sekilSayisi Bulunan sekillerin sayisi.
 */
void sekilleriYazdir(Node* graph,Sekil* sekiller, int sekilSayisi){
    int i,j;
    
	for ( i = 0; i < sekilSayisi; i++){
        printf("%d'gen: ", sekiller[i].uzunluk);
        for ( j = 0; j < sekiller[i].uzunluk + 1; j++){
            printf("%c, ", graph[sekiller[i].nodes[j]].isim);
        }
        printf("\t\t%sÇevresi: %d\n",sekiller[i].uzunluk > 5 ? "\t" : "\t\t", 
        sekiller[i].cevre);
    }
}
