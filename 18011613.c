#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 2415
#define SIZEWORD 5
#define SIZEQUEUE 100

int back[SIZE] = {0};		//iki kelime aras�nda ba�lant� bulunduktan sonra o yol �zerindeki kelimeleri ekrana basmak i�in kullan�lacak matris
char *words[SIZE]; 			//dosyadan okunan t�m kelimelerin yaz�laca�� dizi 
int adj[SIZE][SIZE] = {0};	// adjaceny matrisi

void readFile(){		// kelime.txt dosyas�n� okuyup i�erisindeki kelimeleri *words[] e yazd�ran fonksiyon. main i�erisinde �a��r�l�r.
	FILE *file;
    file = fopen("kelime.txt", "r");		
    char word[8];		//her sat�rda okunan kelimenin yaz�cala�� ge�ici dizi
	int i=0;			//d�ng� de�i�keni
    while(fgets(word, sizeof word, file)!=NULL) {words[i++] = strdup(word);} 
	fclose(file);
}

typedef struct node{		// node isminde struct olu�turur.
	int index;
	struct node* next;
}node;

node *front;			// ilk d���m�n tutulaca�� de�i�ken
node *rear;				// son d���m�n tutulaca�� de�i�ken

node* createNode(int x){					// verilen indis numaras� ile yeni bir d���m olu�turan fonksiyon. enQueue() i�inde �a��r�l�r.
	node *newNode = (node*)malloc(sizeof(node));
	newNode->index = x;
	newNode->next = NULL;
	return newNode;					//yeni olu�turulan d���m� d�nd�r�r.
}

void enQueue(int x){				//verilen indise sahip d���m olu�turularak kuyru�a eklenir.
	node *tmp = createNode(x);
	if (front != NULL) {rear->next = tmp;}		//kuyruk bo� de�il ise 
	else {front = tmp;}							//kuyruk bo� ise
	rear = tmp;
}
void deQueue(){			//kuyru�un ba��ndaki eleman� kuyruktan ��kart�r.
	if (front == NULL) {printf("The queue is already empty ! \n"); return;}
	if (front == rear) {front = NULL; rear = NULL; return;}
	node *tmp2 = front;
	front = tmp2->next;
	//free(tmp2);	
}

int nextInLine(){								// kuyru�un ba��ndaki eleman�n (node) indeksini d�nd�r�r. 
	if (front != NULL) {return front->index;}
	else {printf("The queue is empty !");}
}

int isEmpty(){								// kuyruk bo� ise 0, dolu ise 1 d�nd�r�r.
	if (front == NULL) {return 0;}
	else {return 1;}
}

int compareWord(char a[], char *b){			//verilen iki kelime aras�nda ka� tane farkl� harf oldu�una bakar. 1 tane farkl� harf var ise 1 d�nd�r�r.
	int i;
	int diff=0;
	for(i=0;i<SIZEWORD;i++){
		if (a[i] != b[i]) {diff++;}}
	if (diff == 1) {return 1;}
	else {return 0;}
}
void createAdj(){				// Adjaceny matrisini olu�turan fonksiyon
	int i,j;
	for(i=0;i<SIZE;i++){
		for(j=0;j<SIZE;j++){
			if (compareWord(words[i],words[j]) == 1) {adj[i][j] = 1;}
		}
	}
}

void isLinked(char *a, char *b){		//fonksiyona parametre olarak verilen iki string adjacenty matrisi ile ba�l� m� diye kontrol eder.
	if (adj[findWord(a)][findWord(b)] == 1) { printf("%s and %s are linked!\n",a,b);}
	else {printf("%s and %s are not linked !\n",a,b);}
}

int findWord(char *a){				// verilen string'i *words[] dizisinde arayarak ka��nc� indekste oldu�unu d�nd�r�r. Dizide yok ise -1 d�nd�r�r. 
	int i;
	for(i=0;i<SIZE;i++){
		if (strcmp(a, words[i]) == 0) {return i;}
	}
	//printf("\n The given string does not exist in the file. \n");
	return -1;
}
int bfsFunc(char *a, char *b){			//verilen iki kelime aras�nda ba� olup olmad���n� bulan esas fonksiyon. Parametre olarak iki string al�r.
	if(findWord(a) == -1  || findWord(b) == -1){
		printf("Given words may not exist in the file. Try again .\n"); return NULL;
	}
	int visited[SIZE] = {0};		// grafdaki elemanlar�n�n daha �nce ziyaret edilip edilmedi�ini tutan dizi.
	enQueue(findWord(a)); 	
	visited[findWord(a)] = 1;
	int nextNodeIndex;		//kuyru�un en �n�ndeki eleman�n tuttu�u index
	int i; 			//d�ng� de�i�keni
	while(isEmpty() == 1) {
		nextNodeIndex = nextInLine();		//kuyrukta s�radaki eleman�n indisi  ( grafda her kelimenin words[] dizisindeki indisi tutulur.)
		printf("Kuyruktan cikan : %s", words[nextNodeIndex]);
		deQueue();					//kuyruktan ��kan d���m� ayn� zamanda node1' e atar�z.
		if (strcmp(words[nextNodeIndex], b) == 0) { return nextNodeIndex;}		//verilen b kelimesi bulunursa fonksiyon en son ��kart�lan eleman�n indisini d�nd�r�r ve durur.
		for(i=0;i<SIZE;i++){
			if (adj[nextNodeIndex][i] == 1 && visited[i] == 0 ){
				printf("Kuyruga eklenen : %s", words[i]);
				visited[i] = 1;				
				enQueue(i);
				back[i] = nextNodeIndex;	//her d���m�n kendinden �nceki d���m� bu dizide tutulur.
			}									
		}
		printf("****************************************\n");	
	}
	if (isEmpty() == 0) {printf("There is no transition from %s to %s",a,b); return NULL;}	// verilen iki kelime aras�nda ge�i� yoksa fonksiyon buraya gelir.
}


void printPathv2(int x){	//x de�eri bfsFunc()'�n d�nd�rd��� indistir. Verilen iki kelime aras�ndaki ge�i� s�recinde �zerinden ge�ilen kelimeleri sondan ba�a do�ru ekrana yazd�r�r.
	int count=-1;				//ka� ad�m oldu�unu sayar.
	int f= 0;			// flag. D�ng�y� bitirmek veya devam ettirmek i�in kullan�l�r
	printf(" Yol : ");
	while( f == 0){
		if (back[x] == 0) f = 1;
		printf("%s", words[x]);
		x = back[x]; count++;
	}
	printf("\n %d steps ! \n", count);
}


int main(){									
	int i,j;					//d�ng� de�i�kenleri
	
	readFile();							//kelime.txt dosyas�n� okuyan fonksiyon.
	createAdj();		//Adjaceny matrisini olu�turur.
	
	char first[SIZEWORD+1];			//iki kelime okunur
	char second[SIZEWORD+1];
	scanf("%s", first);
	scanf("%s", second);
	strcat(first, "\n");
	strcat(second, "\n");

	printf("ilk kelimenin indisi : %d ", findWord(first));
	printf(" ikinci kelimenin indisi : %d\n ", findWord(second));
	
	int x = bfsFunc(first, second);
	
	if ( x != NULL)		//verilen iki kelime aras�nda ge�i� varsa ekrana yazd�rma fonksiyonu �a��r�l�r.
		printPathv2(x);
		
	
	return 0;
}





