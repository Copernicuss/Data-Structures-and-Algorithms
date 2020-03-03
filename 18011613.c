#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 2415
#define SIZEWORD 5
#define SIZEQUEUE 100

int back[SIZE] = {0};		//iki kelime arasýnda baðlantý bulunduktan sonra o yol üzerindeki kelimeleri ekrana basmak için kullanýlacak matris
char *words[SIZE]; 			//dosyadan okunan tüm kelimelerin yazýlacaðý dizi 
int adj[SIZE][SIZE] = {0};	// adjaceny matrisi

void readFile(){		// kelime.txt dosyasýný okuyup içerisindeki kelimeleri *words[] e yazdýran fonksiyon. main içerisinde çaðýrýlýr.
	FILE *file;
    file = fopen("kelime.txt", "r");		
    char word[8];		//her satýrda okunan kelimenin yazýcalaðý geçici dizi
	int i=0;			//döngü deðiþkeni
    while(fgets(word, sizeof word, file)!=NULL) {words[i++] = strdup(word);} 
	fclose(file);
}

typedef struct node{		// node isminde struct oluþturur.
	int index;
	struct node* next;
}node;

node *front;			// ilk düðümün tutulacaðý deðiþken
node *rear;				// son düðümün tutulacaðý deðiþken

node* createNode(int x){					// verilen indis numarasý ile yeni bir düðüm oluþturan fonksiyon. enQueue() içinde çaðýrýlýr.
	node *newNode = (node*)malloc(sizeof(node));
	newNode->index = x;
	newNode->next = NULL;
	return newNode;					//yeni oluþturulan düðümü döndürür.
}

void enQueue(int x){				//verilen indise sahip düðüm oluþturularak kuyruða eklenir.
	node *tmp = createNode(x);
	if (front != NULL) {rear->next = tmp;}		//kuyruk boþ deðil ise 
	else {front = tmp;}							//kuyruk boþ ise
	rear = tmp;
}
void deQueue(){			//kuyruðun baþýndaki elemaný kuyruktan çýkartýr.
	if (front == NULL) {printf("The queue is already empty ! \n"); return;}
	if (front == rear) {front = NULL; rear = NULL; return;}
	node *tmp2 = front;
	front = tmp2->next;
	//free(tmp2);	
}

int nextInLine(){								// kuyruðun baþýndaki elemanýn (node) indeksini döndürür. 
	if (front != NULL) {return front->index;}
	else {printf("The queue is empty !");}
}

int isEmpty(){								// kuyruk boþ ise 0, dolu ise 1 döndürür.
	if (front == NULL) {return 0;}
	else {return 1;}
}

int compareWord(char a[], char *b){			//verilen iki kelime arasýnda kaç tane farklý harf olduðuna bakar. 1 tane farklý harf var ise 1 döndürür.
	int i;
	int diff=0;
	for(i=0;i<SIZEWORD;i++){
		if (a[i] != b[i]) {diff++;}}
	if (diff == 1) {return 1;}
	else {return 0;}
}
void createAdj(){				// Adjaceny matrisini oluþturan fonksiyon
	int i,j;
	for(i=0;i<SIZE;i++){
		for(j=0;j<SIZE;j++){
			if (compareWord(words[i],words[j]) == 1) {adj[i][j] = 1;}
		}
	}
}

void isLinked(char *a, char *b){		//fonksiyona parametre olarak verilen iki string adjacenty matrisi ile baðlý mý diye kontrol eder.
	if (adj[findWord(a)][findWord(b)] == 1) { printf("%s and %s are linked!\n",a,b);}
	else {printf("%s and %s are not linked !\n",a,b);}
}

int findWord(char *a){				// verilen string'i *words[] dizisinde arayarak kaçýncý indekste olduðunu döndürür. Dizide yok ise -1 döndürür. 
	int i;
	for(i=0;i<SIZE;i++){
		if (strcmp(a, words[i]) == 0) {return i;}
	}
	//printf("\n The given string does not exist in the file. \n");
	return -1;
}
int bfsFunc(char *a, char *b){			//verilen iki kelime arasýnda bað olup olmadýðýný bulan esas fonksiyon. Parametre olarak iki string alýr.
	if(findWord(a) == -1  || findWord(b) == -1){
		printf("Given words may not exist in the file. Try again .\n"); return NULL;
	}
	int visited[SIZE] = {0};		// grafdaki elemanlarýnýn daha önce ziyaret edilip edilmediðini tutan dizi.
	enQueue(findWord(a)); 	
	visited[findWord(a)] = 1;
	int nextNodeIndex;		//kuyruðun en önündeki elemanýn tuttuðu index
	int i; 			//döngü deðiþkeni
	while(isEmpty() == 1) {
		nextNodeIndex = nextInLine();		//kuyrukta sýradaki elemanýn indisi  ( grafda her kelimenin words[] dizisindeki indisi tutulur.)
		printf("Kuyruktan cikan : %s", words[nextNodeIndex]);
		deQueue();					//kuyruktan çýkan düðümü ayný zamanda node1' e atarýz.
		if (strcmp(words[nextNodeIndex], b) == 0) { return nextNodeIndex;}		//verilen b kelimesi bulunursa fonksiyon en son çýkartýlan elemanýn indisini döndürür ve durur.
		for(i=0;i<SIZE;i++){
			if (adj[nextNodeIndex][i] == 1 && visited[i] == 0 ){
				printf("Kuyruga eklenen : %s", words[i]);
				visited[i] = 1;				
				enQueue(i);
				back[i] = nextNodeIndex;	//her düðümün kendinden önceki düðümü bu dizide tutulur.
			}									
		}
		printf("****************************************\n");	
	}
	if (isEmpty() == 0) {printf("There is no transition from %s to %s",a,b); return NULL;}	// verilen iki kelime arasýnda geçiþ yoksa fonksiyon buraya gelir.
}


void printPathv2(int x){	//x deðeri bfsFunc()'ýn döndürdüðü indistir. Verilen iki kelime arasýndaki geçiþ sürecinde üzerinden geçilen kelimeleri sondan baþa doðru ekrana yazdýrýr.
	int count=-1;				//kaç adým olduðunu sayar.
	int f= 0;			// flag. Döngüyü bitirmek veya devam ettirmek için kullanýlýr
	printf(" Yol : ");
	while( f == 0){
		if (back[x] == 0) f = 1;
		printf("%s", words[x]);
		x = back[x]; count++;
	}
	printf("\n %d steps ! \n", count);
}


int main(){									
	int i,j;					//döngü deðiþkenleri
	
	readFile();							//kelime.txt dosyasýný okuyan fonksiyon.
	createAdj();		//Adjaceny matrisini oluþturur.
	
	char first[SIZEWORD+1];			//iki kelime okunur
	char second[SIZEWORD+1];
	scanf("%s", first);
	scanf("%s", second);
	strcat(first, "\n");
	strcat(second, "\n");

	printf("ilk kelimenin indisi : %d ", findWord(first));
	printf(" ikinci kelimenin indisi : %d\n ", findWord(second));
	
	int x = bfsFunc(first, second);
	
	if ( x != NULL)		//verilen iki kelime arasýnda geçiþ varsa ekrana yazdýrma fonksiyonu çaðýrýlýr.
		printPathv2(x);
		
	
	return 0;
}





