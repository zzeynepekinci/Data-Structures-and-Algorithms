#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 15

struct NODE {
    char* ch; //sayfanin adresini tutar
    int counter;//istek sayisini tutar
    struct NODE *prev;
    struct NODE *next;
};

struct NODE* createNode(char* ch);
void IsInList(struct NODE* node, struct NODE **head, int T, int L);
void control(int L, struct NODE **head, int nodeCount);
void readFromKeyboard(struct NODE **head,int T, int L);
void printList(struct NODE *head);
void clearCache(struct NODE **head);
void readFromFile(struct NODE **head,int T,int L);

int main() {
    struct NODE* head=(struct NODE*)malloc(sizeof(struct NODE));
	head = NULL;
    int c, T, L;
    printf("T=? ");
    scanf("%d",&T);
    printf("L=? ");
    scanf("%d",&L);
    printf("1.keyboard\n2.file\n");
    scanf("%d",&c);
    if (c==1) {
        readFromKeyboard(&head, T, L);
    } else if(c==2) {
        readFromFile(&head, T, L);
    }
    clearCache(&head);
    return 0;
}
/*
@brief linkli listenin bir elemanini olusturur.
@param ch girdi olarak alinan sayfanin adresinin isaretcisidir.
@return olusturulan yapiyi geri dondurur.
*/
struct NODE* createNode(char* ch) {
    struct NODE* newnode = (struct NODE*)malloc(sizeof(struct NODE));
    if (newnode == NULL) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    newnode->next = NULL;//onceki dugum NULL yapilir
    newnode->prev = NULL;
    newnode->ch = strdup(ch);//girdi olarak alinan sayfanin adresi dugumun isaretcisine ataniyor.
    return newnode;
}
/*
@brief elemanin listede olup olmadigini kontrol eder.eger listedeyse sayaci artirir,esik deger gecildiyse elemani head olarak degistirir. listede yoksa yeni eleman ekler.
@param node kullanicidan alinan girdinin ozelliklerini tutan yapiyi isaret eder.
@param **head linkli listenin head isaretcisidir.
@param T esik degerdir.
@param L listenin kapasitesidir.
*/
void IsInList(struct NODE* node, struct NODE **head, int T, int L) {
    int nodeCount=0, flag=0;
    if (*head==NULL) { //liste bossa bu kisim calisir.
        *head=node;
        node->counter = 1;
    } else {
        struct NODE* cur=(struct NODE*)malloc(sizeof(struct NODE));
		cur = *head;
        while (cur!=NULL && flag==0) { //listenin sonuna gelinip gelinmedigi ve elemanin bulunma kontrolu yapilir
            if (strcmp(cur->ch, node->ch) == 0) {//eleman bulunduysa bu blok calisir,sayac artirilir.
                cur->counter++;
                if (cur->counter >T && cur->prev != NULL) {//esik degerin gecip gecilmedigi ve elemanin basta olma kontrolu yapilir.
                    cur->prev->next = cur->next;//elemandan onceki elemanin next isaretcisine elemanin next isaretcisini atar.
                    if (cur->next!=NULL)//elemanin son eleman olup olmadigi kontrolu
                        cur->next->prev = cur->prev;//elemandan sonraki elemanin prev isaretcisine cur elemanin prev isaretcesini atar.
                    cur->next = *head;
                    if (*head != NULL)
                        (*head)->prev = cur;
                    *head = cur;
                }
                free(node->ch); 
                free(node);
                flag = 1;//eleman bulunduysa flag 1 olur.
            }
            cur = cur->next;
            nodeCount++;
        }
        if (cur==NULL && flag==0) { //eleman listede yoksa bu blok calisir.
            node->next=*head;
            if (*head!=NULL) {
                (*head)->prev=node;
            }
            *head=node; //eleman yeni head olarak atanir
            node->counter=1;
            nodeCount++;
            control(L, head, nodeCount);
        }
    }
}
/*
@brief listenin kapasitesinin asilip asilmadigini kontrol eder eger asildiysa liste sonundan bir eleman siler.
@param L listenin kapasitesidir
@param **head listenin en basindaki elemaninin isaretcisidir.
@param nodeCount listede bulunan dugum sayisidir.
*/
void control(int L, struct NODE **head, int nodeCount) {
    if (nodeCount > L) {
        struct NODE* cur=(struct NODE*)malloc(sizeof(struct NODE));
		cur =*head;
        while (cur->next!=NULL) {
            cur=cur->next;
        }
        cur->prev->next=NULL;//son elemandan onceki elemanin next isaretcisine NULL atanir.
        free(cur->ch); 
        free(cur);
    }
}
/*
@brief  klavyeden girdi alinacaksa calisacak fonksiyondur.
@param **head listenin en basindaki elemanin isaretcisidir.
@param T esik degerdir.
@param L listenin kapasitesidir.
*/
void readFromKeyboard(struct NODE **head, int T, int L) {
    char* c;
    c=(char*)malloc(MAX*sizeof(char));
    struct NODE* tmp=(struct NODE*)malloc(sizeof(struct NODE));
    printf("enter addresses, press X for exit: ");
    scanf("%s", c);
    while (strcmp(c,"X") != 0) {
        tmp=createNode(c);
        IsInList(tmp, head, T, L);
        printList(*head);
        scanf("%s", c);
    }
}
/*
@brief cache'i temizleyen fonksiyondur.
@param **head listenin en basindaki elemanin isaretcisidir.
*/
void clearCache(struct NODE **head) {
    int b;
    printf("do you want to clear the cache?\n1. yes\n2. no\n");
    scanf("%d", &b);
    if (b == 1) {
        while (*head!=NULL) {
            struct NODE* temp=(struct NODE*)malloc(sizeof(struct NODE));
		    temp =*head;
            *head =(*head)->next;
            free(temp->ch); // char *ch'yi serbest býrak
            free(temp);
        }
        printf("cache successfully cleared.\n");
    }
}
/*
@brief listeyi ekrana yazdiran fonksiyondur.
@param *head listenin basindaki elemanin isaretcisidir.
*/
void printList(struct NODE *head) {
    struct NODE* cur=(struct NODE*)malloc(sizeof(struct NODE));
	cur = head;
    printf("%s,%d", cur->ch, cur->counter);
    cur = cur->next;
    while (cur!=NULL) {
        printf(" -> <-%s,%d", cur->ch, cur->counter);
        cur = cur->next;
    }
    printf("\n");
}
/*
@brief verilerin dosyadan okunmasini saglayan fonksiyondur
@param **head listenin basindaki elemanin isaretcisidir.
@param T esik degerdir.
@param L listenin kapasitesidir.
*/
void readFromFile(struct NODE **head, int T, int L) {
    char *str; //dosyadan okunacak adresi saklayan isaretci
    char* token;
    str = (char*)malloc(MAX * sizeof(char));
    char* file;
    file = (char*)malloc(MAX * sizeof(char));
    printf("enter the file name: ");
    scanf("%s", file);
    FILE *fp;
    struct NODE* tmp;
    fp = fopen(file, "r");
    if (fp == NULL) {
        printf("Error opening file.\n");
        exit(EXIT_FAILURE);
    }
    while (fgets(str, MAX, fp) != NULL) {//dosya satir satir okunur ve str isaretcisinde tutulur
        token = strtok(str, " \t\n"); //string bosluk ve satir sonu karakterlerinden ayrilir
        while (token != NULL) {
            tmp =createNode(token);//okunan adresle yeni node olusturulur. 
            IsInList(tmp, head, T, L); 
            printList(*head);
            token = strtok(NULL, " \t\n"); 
        }
    }
    fclose(fp);
}


