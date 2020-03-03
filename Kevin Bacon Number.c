#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#define R 5
#define P 7
#define SIZE 250
#define SIZE_ELEMAN 1000        //bir oyuncu max 47 filmde oynyor.
#define SIZE_ELEMAN_MAX 400000 //1000


struct s1;
struct s2;
struct q1;
struct q2;

typedef struct s1{		//film ve oyuncu düğümleri	
	char name[SIZE];		
	struct s1 *array[SIZE_ELEMAN];	//film ise oyuncular, oyuncu ise filmler eklenir.
	int n;	//array içindeki eleman sayısı
	char type;	//film ise 'F', oyuncu ise 'P'
}Node;

Node* createNode(char *name, char X){	//film ise X='F', oyuncu ise X='P'. Yeni düğüm oluşturma			
	if(X == 'F' || X == 'P'){			
		Node *newNode = (Node*)malloc(sizeof(Node));
		strcpy(newNode->name, name);
		newNode->n = 0;
		newNode->type = X;
		return newNode;
	}
	{printf("olusturulamadi. !\n"); return NULL;}
}


typedef struct s2{		//iki oyuncu arasında yol bulunduktan sonra geriye dönüş bilgisini tutan struct yapısı.
	int back_index;
	char f_name[SIZE];
}Back;


Back* createBackNode(int x, char* aName){	
	Back *tmp = (Back*)malloc(sizeof(Back));
	tmp->back_index = x;
	strcpy(tmp->f_name, aName);
	return tmp;
}

typedef struct q1 {			//Kuyruk structı
    struct q2 *front, *rear;
}Queue;

Queue* createQueue() {		//Kuyruk oluşturma fonksiyonu.
    Queue* tmp = (Queue*)malloc(sizeof(Queue));
    tmp->front = tmp->rear = NULL;			//kuyruk baş-son bilgisi.
    return tmp;
}

typedef struct q2{		//Kuyruğa atılacak oyuncu-film düğümleri, kuyruk düğümü içerisine koyulur.
	Node *node;
	struct q2* next;
}QueueNode;

QueueNode* createQueueNode(Node* aNode) {	//Kuyruğa atılacak oyuncu-film düğümleri, kuyruk düğümü içerisine koyulur.
    QueueNode* tmp = (QueueNode*)malloc(sizeof( QueueNode));
    tmp->next = NULL;
	tmp->node = aNode;
    return tmp;
}

void enQueue(Queue *myqueue, Node *aNode){	//kuyruğa yerleştirme fonksiyonu
    QueueNode* tmp_qnode = createQueueNode(aNode);
    if (myqueue->rear == NULL) {
        myqueue->front = tmp_qnode;
		myqueue->rear = tmp_qnode;
        return;
    }
    myqueue->rear->next = tmp_qnode;
    myqueue->rear = tmp_qnode;
}

Node* next(Queue *myqueue){		//kuyrukta en öndeki elemanı döndürür
	if (myqueue->front == NULL)  { printf("the queue is empty \n");  return NULL; }
	return myqueue->front->node;
}

bool deQueue(Queue *myqueue){   //en öndeki elemanı kuruktan çıkarır.
	if (myqueue->front == NULL)  { return false;  }
    QueueNode* tmp_qnode = myqueue->front;
    myqueue->front = myqueue->front->next;
    if (myqueue->front == NULL) { myqueue->rear = NULL; }
    tmp_qnode->node = NULL;
    free(tmp_qnode);
    return true;
}

void printQueue(Queue *myqueue) {	//kuyruğun o anki durumunu ekrana basar. İhtiyaç durumunda kullanılabilir. Program içinde herhangi bir yerde çağrılmamıştır.
	QueueNode* tmp = myqueue->front;
	if(tmp==NULL) {printf("the queue is empty !\n");  return;}
	while(tmp != NULL) {
		printf("%s \n",tmp->node->name);
		tmp = tmp->next;
	}
	printf("\n");
}


void connectNodes(Node *film, Node *player){	//Bir oyuncu ve bir film düğümünü birbirine bağlar
	film->array[film->n] = player;
	film->n++;
	player->array[player->n] = film;
	player->n++;
	
}

long long int calcHashNum(char *aName){		//hash fonksiyonu için hesaplama yapar.
	char *tmp = strdup(aName);
	int length = strlen(tmp);
	int i=0;
	long long int num=0;
	for(i=0; i<length; i++){
		num = num + (long long int)( abs(tmp[i]) *  (int)pow( (double)R , (double)((length-i)%P) ) );
	}
	//printf("num : %d \n", num);
	return num;
}

void addToHash(Node *aNode ,Node** HashTable ){		//verilen düğümü hash tablosuna yerleştirir.
	long long int index = calcHashNum(aNode->name);	// elde edilen indeks boş ise oraya yerleştirir. boş değil ise i 1 artırılır.
	bool flag = false;
	int i = 0;
	int key;										// Quadratic hashing kullanılmıştır.
	while(flag== false){
        key = ( (index+i*i) % (SIZE_ELEMAN_MAX-1));

		if (HashTable[key] == NULL){
			HashTable[key] = aNode;
			flag = true;
		}
		else{ i++; }
	}
}


int searchInHash(char *aName, Node** HashTable ){	//Verilen isime sahip düğüm, hash tablosunda var mı diye bakar.Varsa indeksini, yoksa -1 döndürür.
	long long int index = calcHashNum(aName);
	bool flag = false;
	int i = 0;
	int key = ((index+i*i) % (SIZE_ELEMAN_MAX-1));
	
	while(HashTable[key] != NULL  ){
		if( strcmp(HashTable[key]->name, aName) == 0) {   return key; }
		i++;
		key = ( (index+i*i) % (SIZE_ELEMAN_MAX-1));
	}
	//printf("eslesmedi. \n");
	return -1;
}

void printRecursive(int x, Back** back, Node** HashTable, int* c, int* first_index){	//printPathV2() içinde çağrılır. Back pointer dizisi içerisinde sondan başa
	if(back[x]->back_index == -1) 	*(first_index) = x;									//doğru ilerleyip ekrana bastırır.
	if (back[x]->back_index != -1){
		(*c)++;
		printRecursive(back[x]->back_index , back, HashTable, c,first_index );
		printf("%s - %s   '%s' \n", HashTable[back[x]->back_index]->name, HashTable[x]->name, back[x]->f_name);
	}
}


void printPathv2( int last_index, Back** back , Node** HashTable){		//iki oyuncu arasında yol bulduğunda son oyuncudan ilk oyuncuya yolu ekrana bastırır.
	int count=0;
	int first_index;
	int x = last_index;
	printf(" \n");
	printRecursive(x, back, HashTable, &count, &first_index);
	if(count > 6){					
		printf("\n%s  -  %s  arasında bacon sayısı yoktur. \n",HashTable[first_index]->name, HashTable[last_index]->name); return;
	}
	printf("\n%s  -  %s  : %d \n",HashTable[first_index]->name, HashTable[last_index]->name,  count);
}

void clearQueue(Queue *myqueue){
	bool f = deQueue(myqueue);
	while(f == true){ 
		deQueue(myqueue);
		f = deQueue(myqueue);
	}
}

void BFS(Node *source_node, Node *destination_node, Back** back ,  Node** HashTable ){	//İki oyuncunun düğümlerini parametre olarak alır.
	int i;		
	int tmp;		
	int hashTableIndex;		// Oyuncu1 -> Film -> Oyuncu2  	--bir oyuncu kuyruktan çıkarılırken ona bir film ile bağlantılı oyuncular arasındaki bağı tutmak için kullanılmıştır.
	int *visited = calloc(SIZE_ELEMAN_MAX, sizeof(int));	//Düğümlerin ziyaret edilip edilmediği bilgisini tutar. 
	
	Queue *myqueue = createQueue();
	enQueue(myqueue, source_node);				//i1 i2
	
	tmp = searchInHash(source_node->name, HashTable);
	visited[tmp] = 1;	//verilen ilk oyuncunun düğümü visited yapılır.
	back[tmp] = createBackNode(-1, "---------" /*source_node->name*/ );	//ilk düğümden öncesi olmadığı için -1 verilir. "-------" nin bir anlamı yok.
	

	while(myqueue->front != NULL){
		Node *out_node = next(myqueue);	//kuyruğun en önündeki eleman ayrı bir değişkende tutulur.
		deQueue(myqueue);		//kuyruğun en önündeki eleman düğümden çıkarılır.

		if(out_node->type == 'P') {	// Oyuncu1 -> Film -> Oyuncu2  	--bir oyuncu kuyruktan çıkarılırken ona bir film ile bağlantılı oyuncular arasındaki bağı tutmak için kullanılmıştır.
			hashTableIndex = searchInHash(out_node->name, HashTable);
		} 	//back traverse }

		//printf("yol  : %s \n", out_node->name);
		
		if(out_node == destination_node){		//iki düğüm arasında yol bulunmuş ise ekrana bastırılır. BFS sonlandırılır.
			printPathv2(hashTableIndex,back,HashTable );
			free(visited);
			clearQueue(myqueue);
			free(myqueue);
			return;
		}
		for(i=0; i<out_node->n  ; i++){		//kuruktan çıkarılan düğüme bağlı düğümler kuyruğa eklenir.
			if( visited[searchInHash(out_node->array[i]->name, HashTable)] == 0 ){
				enQueue(myqueue, out_node->array[i] );		
				visited[searchInHash(out_node->array[i]->name, HashTable)] = 1;
				if(out_node->array[i]->type == 'P') {
					back[searchInHash(out_node->array[i]->name, HashTable)] = createBackNode(hashTableIndex, out_node->name);	//back dizisinde son iki oyuncu arasında bağ konulur.
				}
			}
		}
	}
	free(visited);
	free(myqueue);
	printf(" Bulunamadı !\n");
}

void generatorF(char* oneLine, Node** HashTable){	// her satır her / için ayrılır. Böylece isimlere ulaşılır. İlk isim filmdir. Diğerleri oyuncular.
	int flag = 0;		//ilk başta filme gitmesi için kullanılır. Film okunduktan sonra 1 olur.
	int sign;
	Node* tmp_fnode; 	//geçiçi düğümler
	Node* tmp_pnode;	
	char* tmp;
	tmp = strtok(oneLine, "/");
	
	while(tmp != NULL){
        if( strstr(tmp, "\n") ){tmp = strtok(tmp,"\n");}	//satır sonundaki new line 'ı atmak için
        
		if(flag==0){
			tmp_fnode = createNode(tmp, 'F');	//film düğümü oluşturulur. Hash tablosuna eklenir.
			addToHash(tmp_fnode, HashTable);
			flag = 1;
		}
		else{
			sign = searchInHash(tmp, HashTable);	//oyuncu okunur. hash tablosunda var mı diye kontrol edilir.
			if (sign == -1){
				tmp_pnode = createNode(tmp, 'P');	//yoksa oluşturulur ve hash tablosuna eklenir.
				addToHash(tmp_pnode, HashTable);
			}
			else{		
				tmp_pnode = HashTable[sign];		//hash tablosunda varsa , düğüme ulaşılır.
			}
			connectNodes(tmp_fnode, tmp_pnode);		//iki düğüm birbirine bağlanır
		}
		tmp = strtok(NULL, "/");	// ? \n ?
	}
}


void readFile(Node** HashTable){	//dosya okunur
	FILE *ptrfile;
	ptrfile = fopen("input-mpaa.txt", "r");
	if (ptrfile == NULL)  {printf("no file is found. \n");  return; }
	char oneLine[5000];	//max:3709	//okunan satırlar bu dizilere atılır.
	int max=0;
	while( fgets(oneLine, 5000, ptrfile) != NULL){
		generatorF(oneLine, HashTable);		//satır satır okunur. Her satır fgets() e gönderilir.
		memset(oneLine, 0, sizeof(oneLine));
	}
	fclose(ptrfile);
}


void removeNL(char* string){					//fgets ile kullanıcıdan input alınırken string in sonuna eklenen new line dan kurtulmak için kullanılır.
    unsigned int length = strlen(string);
    if((length > 0) && (string[length-1] == '\n')){
        string[length-1] ='\0';
    }
}



int main(){

	Node** HashTable = calloc(SIZE_ELEMAN_MAX, sizeof(Node*));	
	Back** back = calloc(SIZE_ELEMAN_MAX, sizeof(Back*));		
				
    printf("Dosya okunuyor... \n\n");
	readFile( HashTable );
	
	int loop=1;
	int selection;	
	char s1[SIZE];
	char s2[SIZE];
	char s3[SIZE] = "Bacon, Kevin";
	int a,b;
	char bos = NULL;
	
	printf("*Not : Oyuncu ismini 'Soyad, Ad' formatinda girmelisiniz ! Ornegin : 'Bacon, Kevin' *\n\n");
	printf("  ------- MENU ------- \n");
	printf("Kevin Bacon sayisini bulmak icin ------->  1\nHerhangi iki oyuncu arasindaki mesafeyi bulmak icin ----> 2\nProgrami kapatmak icin --------> 9\n");
	printf("  ------------------ \n");
	while(1){
		printf("-------------------------------------------------\n");
		printf("Seciminiz :");
		scanf("%d",&selection);
		scanf("%c", &bos);
		
		if(selection == 1){
			printf(" Oyuncuyu 'Soyad, Ad' seklinde giriniz :");
			fgets(s1,SIZE,stdin);
			removeNL(s1);
			a = searchInHash(s1, HashTable);
			b = searchInHash(s3, HashTable);
			if(a != -1 && b!= -1) { BFS( HashTable[a],  HashTable[b] , back, HashTable);}
			else{ printf("Veritabanında bu isimde bir oyuncu yok. Ya da oyuncu ismini istenen sekli ile girmediniz. Yeniden deneyiniz. \n");}
			//memset(s1, 0, sizeof(s1));
		}
		else if(selection == 2){
			printf(" Oyuncuyu 'Soyad, Ad' seklinde giriniz :");
			fgets(s1,SIZE,stdin);
			removeNL(s1);
			a = searchInHash(s1, HashTable);
			if(a == -1) { printf("\nVeritabanında bu isimde bir oyuncu yok. Yeniden oyuncu giriniz. \n"); continue;}
			printf(" Ikinci oyuncuyu 'Soyad, Ad' seklinde giriniz :");
			fgets(s2,SIZE,stdin);
			removeNL(s2);
			b = searchInHash(s2, HashTable);
			if(a != -1 && b!= -1) { BFS( HashTable[a],  HashTable[b] , back, HashTable);}
			else {printf("Veritabanında bu isimde bir oyuncu yok. Yeniden oyuncu giriniz. \n");}
		}
				
		else if(selection == 9){
			free(HashTable);
			free(back);
			exit(0);
		}
		else{printf("Programdan cikmak icin 9'a basiniz.\n");}
	}	
	
	
	/*
	char inpt1[SIZE] = "Willcox, Tim";
	char inpt2[SIZE] = "Baker, Shanna";    // Boucher, Philip    Times, Themsi    Baker, Shanna    Willcox, Tim
	
	int in1 = searchInHash(inpt1, HashTable);
	int in2 = searchInHash(inpt2, HashTable);
	//printf("%d   %d  \n", in1, in2);
	if(in1 != -1 && in2 != -1){
		BFS( HashTable[in1],  HashTable[in2] , back, HashTable);
	}
	else{
		printf("yeniden oyuncu giriniz. \n");
	}
	*/

	return 0;
}




