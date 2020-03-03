#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 2415
#define SIZEWORD 5
#define SIZEQUEUE 100

int back[SIZE] = {0};		//iki kelime arasında bağlantı bulunduktan sonra o yol üzerindeki kelimeleri ekrana basmak için kullanılacak matris
char *words[SIZE]; 			//dosyadan okunan tüm kelimelerin yazılacağı dizi 
int adj[SIZE][SIZE] = {0};	// adjaceny matrisi

void readFile(){		// kelime.txt dosyasını okuyup içerisindeki kelimeleri *words[] e yazdıran fonksiyon. main içerisinde çağırılır.
	FILE *file;
    file = fopen("kelime.txt", "r");		
    char word[8];		//her satırda okunan kelimenin yazıcalağı geçici dizi
	int i=0;			//döngü değişkeni
    while(fgets(word, sizeof word, file)!=NULL) {words[i++] = strdup(word);} 
	fclose(file);
}

typedef struct node{		// node isminde struct oluşturur.
	int index;
	struct node* next;
}node;

node *front;			// ilk düğümün tutulacağı değişken
node *rear;				// son düğümün tutulacağı değişken

node* createNode(int x){					// verilen indis numarası ile yeni bir düğüm oluşturan fonksiyon. enQueue() içinde çağırılır.
	node *newNode = (node*)malloc(sizeof(node));
	newNode->index = x;
	newNode->next = NULL;
	return newNode;					//yeni oluşturulan düğümü döndürür.
}

void enQueue(int x){				//verilen indise sahip düğüm oluşturularak kuyruğa eklenir.
	node *tmp = createNode(x);
	if (front != NULL) {rear->next = tmp;}		//kuyruk boş değil ise 
	else {front = tmp;}							//kuyruk boş ise
	rear = tmp;
}
void deQueue(){			//kuyruğun başındaki elemanı kuyruktan çıkartır.
	if (front == NULL) {printf("The queue is already empty ! \n"); return;}
	if (front == rear) {front = NULL; rear = NULL; return;}
	node *tmp2 = front;
	front = tmp2->next;
	//free(tmp2);	
}

int nextInLine(){								// kuyruğun başındaki elemanın (node) indeksini döndürür. 
	if (front != NULL) {return front->index;}
	else {printf("The queue is empty !");}
}

int isEmpty(){								// kuyruk boş ise 0, dolu ise 1 döndürür.
	if (front == NULL) {return 0;}
	else {return 1;}
}

int compareWord(char a[], char *b){			//verilen iki kelime arasında kaç tane farklı harf olduğuna bakar. 1 tane farklı harf var ise 1 döndürür.
	int i;
	int diff=0;
	for(i=0;i<SIZEWORD;i++){
		if (a[i] != b[i]) {diff++;}}
	if (diff == 1) {return 1;}
	else {return 0;}
}
void createAdj(){				// Adjaceny matrisini oluşturan fonksiyon
	int i,j;
	for(i=0;i<SIZE;i++){
		for(j=0;j<SIZE;j++){
			if (compareWord(words[i],words[j]) == 1) {adj[i][j] = 1;}
		}
	}
}

void isLinked(char *a, char *b){		//fonksiyona parametre olarak verilen iki string adjacenty matrisi ile bağlı mı diye kontrol eder.
	if (adj[findWord(a)][findWord(b)] == 1) { printf("%s and %s are linked!\n",a,b);}
	else {printf("%s and %s are not linked !\n",a,b);}
}

int findWord(char *a){				// verilen string'i *words[] dizisinde arayarak kaçıncı indekste olduğunu döndürür. Dizide yok ise -1 döndürür. 
	int i;
	for(i=0;i<SIZE;i++){
		if (strcmp(a, words[i]) == 0) {return i;}
	}
	//printf("\n The given string does not exist in the file. \n");
	return -1;
}
int bfsFunc(char *a, char *b){			//verilen iki kelime arasında bağ olup olmadığını bulan esas fonksiyon. Parametre olarak iki string alır.
	if(findWord(a) == -1  || findWord(b) == -1){
		printf("Given words may not exist in the file. Try again .\n"); return NULL;
	}
	int visited[SIZE] = {0};		// grafdaki elemanlarının daha önce ziyaret edilip edilmediğini tutan dizi.
	enQueue(findWord(a)); 	
	visited[findWord(a)] = 1;
	int nextNodeIndex;		//kuyruğun en önündeki elemanın tuttuğu index
	int i; 			//döngü değişkeni
	while(isEmpty() == 1) {
		nextNodeIndex = nextInLine();		//kuyrukta sıradaki elemanın indisi  ( grafda her kelimenin words[] dizisindeki indisi tutulur.)
		printf("Kuyruktan cikan : %s", words[nextNodeIndex]);
		deQueue();					//kuyruktan çıkan düğümü aynı zamanda node1' e atarız.
		if (strcmp(words[nextNodeIndex], b) == 0) { return nextNodeIndex;}		//verilen b kelimesi bulunursa fonksiyon en son çıkartılan elemanın indisini döndürür ve durur.
		for(i=0;i<SIZE;i++){
			if (adj[nextNodeIndex][i] == 1 && visited[i] == 0 ){
				printf("Kuyruga eklenen : %s", words[i]);
				visited[i] = 1;				
				enQueue(i);
				back[i] = nextNodeIndex;	//her düğümün kendinden önceki düğümü bu dizide tutulur.
			}									
		}
		printf("****************************************\n");	
	}
	if (isEmpty() == 0) {printf("There is no transition from %s to %s",a,b); return NULL;}	// verilen iki kelime arasında geçiş yoksa fonksiyon buraya gelir.
}


void printPathv2(int x){	//x değeri bfsFunc()'ın döndürdüğü indistir. Verilen iki kelime arasındaki geçiş sürecinde üzerinden geçilen kelimeleri sondan başa doğru ekrana yazdırır.
	int count=-1;				//kaç adım olduğunu sayar.
	int f= 0;			// flag. Döngüyü bitirmek veya devam ettirmek için kullanılır
	printf(" Yol : ");
	while( f == 0){
		if (back[x] == 0) f = 1;
		printf("%s", words[x]);
		x = back[x]; count++;
	}
	printf("\n %d steps ! \n", count);
}


int main(){									
	int i,j;					//döngü değişkenleri
	
	readFile();							//kelime.txt dosyasını okuyan fonksiyon.
	createAdj();		//Adjaceny matrisini oluşturur.
	
	char first[SIZEWORD+1];			//iki kelime okunur
	char second[SIZEWORD+1];
	scanf("%s", first);
	scanf("%s", second);
	strcat(first, "\n");
	strcat(second, "\n");

	printf("ilk kelimenin indisi : %d ", findWord(first));
	printf(" ikinci kelimenin indisi : %d\n ", findWord(second));
	
	int x = bfsFunc(first, second);
	
	if ( x != NULL)		//verilen iki kelime arasında geçiş varsa ekrana yazdırma fonksiyonu çağırılır.
		printPathv2(x);
		
	
	return 0;
}





