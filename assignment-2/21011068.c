#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 30

struct SNode { //stackteki dugum yapisidir.
    char* kelime;
    struct SNode* next;
};

struct Stack {
    struct SNode* top;
};

struct QNode { //kuyruktaki dugum yapisidir.
	struct Stack* s;
    struct QNode* next;
};

struct Queue {
    struct QNode *front, *rear;
};
/*
@brief stack olusumu icin kullanilan bellekleri serbest birakir.
@param stack islem yapilacak stacktir.
*/
void freeStack(struct Stack* stack) {
    if (stack == NULL)
        return;
    struct SNode* current = stack->top;
    struct SNode* nextNode;

    while (current != NULL) {
        nextNode = current->next;
        free(current->kelime);
        free(current);
        current = nextNode;
    }
    free(stack);
}
/*
@brief kuyruk olusumu icin kullanilan bellekleri serbest birakir.
@param q islem yapilacak kuyruk yapisidir.
*/
void freeQueue(struct Queue* q) {
    if (q == NULL)
        return;
    struct QNode* current = q->front;
    struct QNode* nextNode;

    while (current != NULL) {
        nextNode = current->next;
        freeStack(current->s);
        free(current);
        current = nextNode;
    }
    free(q);
}
/*
@brief stack icin dugum olusturur.
@param kelime dugumdeki kelimedir.
@return olusan dugumu geri dondurur.
*/
struct SNode* createNode(char* kelime) {
    struct SNode* newNode = (struct SNode*)malloc(sizeof(struct SNode));
    newNode->kelime = strdup(kelime);
    newNode->next = NULL;
    return newNode;
}
/*
@brief stacke kelime eklenmesini saglar
@param stack eklenecek stacktir.
@param kelime eklenecek kelimedir.
*/
void push(struct Stack* stack, char* kelime) {
    struct SNode* newNode = createNode(kelime);
    newNode->next = stack->top;
    stack->top = newNode;
}
/*
@brief stackten kelime cikarmayi saglar.
@param stack kelimenin cikarilacagi stacktir.
@return cikarilan kelimeni adresini dondurur.
*/
char* pop(struct Stack* stack) {
    if (stack->top == NULL) {
        printf("Stack is empty.");
        return NULL;
    }
    char* poppedKelime= stack->top->kelime;
    struct SNode* temp= stack->top;
    stack->top= stack->top->next;
    free(temp);
    return poppedKelime;
}
/*
@brief stackte bulunan en ustteki kelimeye erisimi saglar.
@param stack kelimenin erisilecegi stacktir.
@return stackin en ustundeki kelimenin adresini dondurur.
*/
char* peek(struct Stack* stack) {
    if (stack->top == NULL) {
        printf("Stack is empty.");
        return NULL;
    }
    return stack->top->kelime;
}
/*
@brief kuyruk icin dugum olusturur.
@param stack kuyruktaki stack yapisidir.
@return olusan dugumu dondurur.
*/

struct QNode* newNode(struct Stack* stack) {
    struct QNode* temp = (struct QNode*)malloc(sizeof(struct QNode));
    temp->s= stack;
    temp->next= NULL;
    return temp;
}
/*
@brief kuyruk olusmasini saglar.
@return olusan kuyrugu dondurur.
*/

struct Queue* createQueue() {
    struct Queue* q= (struct Queue*)malloc(sizeof(struct Queue));
    q->front= q->rear = NULL;
    return q;
}
/*
@brief kuyruga eleman eklemeyi saglar.
@param q elemanin eklenecegi kuyruktur.
@param stack kuyruga eklenecek dugumun yapisini olusturan stacktir.
*/
void enqueue(struct Queue* q, struct Stack* stack) {
    struct QNode* temp= newNode(stack);
    if (q->rear== NULL) {
        q->front= q->rear = temp;
        return;
    }
    q->rear->next = temp;
    q->rear = temp;
}
/*
@brief kuyruktan eleman cikarmayi saglar.
@param q elemanin cikarilacagi kuyruktur.
*/

void dequeue(struct Queue* q) {
    if (q->front == NULL) {
        printf("Queue is empty!\n");
        return;
    }
    struct QNode* temp = q->front;
    q->front = q->front->next;
    if (q->front == NULL)
        q->rear = NULL;
    free(temp);
}
/*
@brief kuyrugun en basindaki elemana erisimi saglar;
@param q elemanin erisilevegi kuyruktur.
@return kuyrugun en basindaki stack yapisinin adresini dondurur.
*/
struct Stack* peekQueue(struct Queue* q) {
    if (q->front == NULL) {
        printf("Queue is empty!\n");
        return NULL;
    }
    return q->front->s;
}
/*
@brief kuyrugun dolulugunu kontrol eder.
@param dolulugun kontrol edilecegi kuyruktur.
@return bossa 1, doluysa 0 dondurur.
*/
int isEmpty(struct Queue* q) {
    return (q->front == NULL);
}
/*
@brief iki kelime arasindaki farkli harf sayisini dondurur.
@param a ve b kontrol edilecek kelimelerin adresidir.
@return farkli harf sayisini dondurur.
*/
int farkBul(char *a,char *b){
    int i=0,fark=0;
    while(a[i]!='\0' ){
        if(a[i]!=b[i]){
            fark++;
        }
        i++;
    }
    return fark;
}

/*
@brief stackin kopyalanmasini saglar.
@param stack kopyalanacak stacktir.
@return kopyalanan stacki dondurur.
*/
struct Stack* stackKopyala(struct Stack* stack){
    struct Stack* tempStack = (struct Stack*)malloc(sizeof(struct Stack));
    struct Stack* s = (struct Stack*)malloc(sizeof(struct Stack));
	if (stack == NULL || stack->top == NULL) {
        printf("Stack is empty.\n");
        return NULL;
    }
    tempStack->top = NULL;
    struct SNode* current = stack->top;//kopyalanacak stackin en ustundeki eleman currenta esitleniyor.
    while (current != NULL) {
        push(tempStack, current->kelime);
        current = current->next;
    }
    current = tempStack->top;//gecici stack'in top'i current'a esitlenir.
    struct SNode* newTop = NULL;//kopya stackin top'i;
    while (current != NULL) {//bu blokta gecici stackteki kelimeler kopya stack'e eklenir
        struct SNode* newNode = createNode(current->kelime);
        newNode->next = newTop;
        newTop = newNode;
        current = current->next;
    }
    s->top = newTop;
    freeStack(tempStack);
    return s;
}
/*
@brief dosyada kaynak ve hedef kelimenin varligini kontrol eder.
@param hedef hedef kelimedir.
@param kaynak kaynak kelimedir.
*/
int dosyadaKelimeKontrolu(char* hedef,char* kaynak){
	char *str,*token;
	str = (char*)malloc(MAX* sizeof(char));
    token = (char*)malloc(MAX* sizeof(char));
	int kontrol=0;
	FILE *fp;
    fp=fopen("dictionary.txt","r");
    if (fp == NULL) {
        printf("Error opening file.\n");
        exit(EXIT_FAILURE);
    }
    while (fgets(str, MAX, fp) != NULL) {
			token = strtok(str, " \t\n");
            while (token != NULL ) {
            	if(strcmp(hedef,token)==0 || strcmp(kaynak,token)==0){
            		kontrol++;
				}
                token = strtok(NULL, " \t\n");
            }
    }
    fclose(fp);
    return kontrol;
}
/*
@brief kullanicidan kaynak ve hedef kelimenin alinmasini saglayan fonksiyondur.
@param kaynakKelime kaynak kelimenin tutulacagi adresin isaretcisidir.
@param hedefKelime hedef kelimenin tutulacagi adresin isaretcisidir.
*/
void kelimeleriAl(char* kaynakKelime, char* hedefKelime){
    printf("kaynak kelime: ");
    scanf("%s", kaynakKelime);
    printf("hedef kelime: ");
    scanf("%s", hedefKelime);
}

/*
@brief stacklere eklenecek kelimenin daha once eklenip eklenmedigi kontrolunu yapar.
@param kelimeler stacke eklenen kelimeleri tutan isaretcidir.
@param n stacke eklenenen benzersiz kelime sayisidir
@return kelime varsa 0,yoksa 1 dondurur.
*/
int benzersizKelimeKontrolu(char** kelimeler,int n,char* token){
    int i;
    for(i=0;i<n;i++){
        if(strcmp(token,kelimeler[i])==0){
            return 0;
        }
    }
    return 1;
}
/*
@brief stackte bulunan kelimeleri ekrana yazdirir.
@param *s ekrana yazdirilacak stacktir. 
*/
void printStack(struct Stack* s){
    
	char* kelime;
	struct SNode* node= createNode(kelime);
    struct Stack* tempStack = (struct Stack*)malloc(sizeof(struct Stack));
    tempStack->top = NULL;
    while(s->top!=NULL){//stack bosalana kadar bu blok calisir.
        node->kelime=peek(s);
        push(tempStack,node->kelime);
        pop(s);
    }
    node = tempStack->top;
    while(node != NULL){
        printf(" %s ", node->kelime);
        node = node->next;
    }
    printf("\n");
    freeStack(tempStack);
}
/*
@brief hedef ve kaynak kelimenin harf sayisina sahip olan kelimelerin baska dosyaya yazdirilma islemidir. 
*/
void yeniDosya(char* kaynakKelime) {
    FILE *okunanDosya, *yazilanDosya;
    char kelime[MAX];
    okunanDosya = fopen("dictionary.txt", "r");
    if (okunanDosya == NULL) {
        printf("Error opening file!\n");
        return; 
    }
    yazilanDosya = fopen("yeniDosya.txt", "w");
    if (yazilanDosya == NULL) {
        printf("error opening file!\n");
        fclose(okunanDosya); 
        return; 
    }
    while (fscanf(okunanDosya, "%s", kelime) == 1) {
        if (strlen(kelime) == strlen(kaynakKelime)) {
            fprintf(yazilanDosya, "%s\n", kelime);
        }
    }
    fclose(okunanDosya);
    fclose(yazilanDosya);
}

/*
@brief kelimeleri dosyadan okumaya yarayan ve diger fonksiyonlarin kullanildigi fonksiyondur.
@param kelime kaynak kelimenin isaretcisidir.
@param hkelime hedef kelimenin isaretcisidr.

*/

void kelimeMerdiveni(char* kelime,char* hkelime){
    int  farkliHarfSayisi=1,i=0,kontrolFark,kelimeKontrolu=0,j;
    char str[MAX],*token,*kontrol;
    struct Stack *s=(struct Stack* )malloc(sizeof(struct Stack));
    struct Queue* q=(struct Queue*)malloc(sizeof(struct Queue)) ;
    struct Stack *tmp=(struct Stack* )malloc(sizeof(struct Stack));
    struct Stack *stack=(struct Stack* )malloc(sizeof(struct Stack));
    FILE *fp;
	char **kelimeler;
    kelimeler=(char**)malloc(sizeof(char*)*20000);
	q=createQueue();
    for(j=0;j<20000;j++){
        kelimeler[j]=(char*)malloc(sizeof(char)*MAX);
    }
    yeniDosya(kelime);
	fp=fopen("yeniDosya.txt","r");
    if (fp == NULL) {
        printf("Error opening file.\n");
        exit(EXIT_FAILURE);
    }
    tmp->top = NULL;
    push(tmp, kelime);
    enqueue(q, tmp);
    kontrol = peek(tmp);
    if(strlen(kelime)==strlen(hkelime)){
    	kelimeKontrolu=dosyadaKelimeKontrolu(kelime,hkelime);
	}
 
    while ( kelimeKontrolu==2 && farkliHarfSayisi != 0) {
        while (fgets(str, MAX, fp) != NULL) {
			token = strtok(str, " \t\n");
            while (token != NULL ) {
                kontrolFark = farkBul(kontrol, token);
                if (kontrolFark == 1 && benzersizKelimeKontrolu(kelimeler, i, token)) {
                    strcpy(kelimeler[i], token);
                    i++;
                    s = stackKopyala(tmp);
                    push(s, token);
                    /*stack=stackKopyala(s);   //kuyruktaki stacklerin yazdirilmasi icin bu kisim calismali.
                    printStack(stack);
                    
                    freeStack(stack);*/
                    enqueue(q, s);
                    
                }
               
                token = strtok(NULL, " \t\n");
            }
        }
        rewind(fp);
        dequeue(q);
		freeStack(tmp);
		
		tmp = peekQueue(q);
        kontrol = peek(tmp);
        farkliHarfSayisi = strcmp(kontrol,hkelime);
    }
    if(strcmp(kontrol,hkelime) == 0 && kelimeKontrolu==1 ){
    	printf("kelimeler aynidir.");
	}
    else if (strcmp(kontrol,hkelime) == 0 ) {
        printStack(tmp);
    }
    else if(kelimeKontrolu==0){
		printf("gecersiz kelimeler girdiniz.");
	}
    else if(kelimeKontrolu!=2){
    	printf("kelime sozlukte bulunmuyor");
	}
    else if (isEmpty(q)) {
        printf("kelime merdiveni yoktur.");
    }
		
    fclose(fp);
    freeQueue(q);
    free(kelimeler);
}

int main() {
    char kaynakKelime[MAX];
    char hedefKelime[MAX];
    kelimeleriAl(kaynakKelime, hedefKelime);
    kelimeMerdiveni(kaynakKelime, hedefKelime);
    return 0;
}
