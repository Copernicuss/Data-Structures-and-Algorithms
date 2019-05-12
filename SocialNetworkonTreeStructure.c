/*    Sample Input File

31,Ayse Terim,9-15-53-60-29-32-43-59
9,Gokhan Yeter,29-17-49
15,Banu Genc,3-12
53,Vakkas Demir,24-38-7-54
60,Sibel Efe,62-46-59
29,Nur Yeter,16-4
17,Bekir Gun,25-23-32-1
49,Mukaddes Hicran,43-41-52
3,Abdullah Ilk,40-31
12,Burak Tanır,9-15-53-60
24,Tulay Korkmaz,29-17-49

*/

#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#define SIZE 50

typedef struct node{
	int id;
	char name[SIZE];
	int mates[SIZE];
	struct node *left;
	struct node *right;
}Person;

int readFile(int x,int *id,char *name,int *friends){	//bu fonksiyon içinde dosya okunur. 1 veya 0 döndürür. x değeri Input dosyasında aranacak id numarasıdır.
	FILE *ptrfile;										//x bir id ile eşlenirse fonksiyon 0 döndürür. Parametre olarak aldığı pointer'ların işaret ettiği adreslere 
	ptrfile = fopen("Input.txt", "r");					// Input dosyası içindeki ilgili bilgileri yazar. x bir id ile eşleşmez ise fonksiyon 1 döndürür.
	if (ptrfile == NULL){
		printf("no file is found.");
	}
	char text[SIZE];	//Input dosyası içerisindeki kişilerin isimleri ve arkadaş dizileri ilk aşamada bu dizi içerisine yazılır.
	int sign = 1;		//Input dosyası içerisinde verilen id bulunursa sign = 0 yapılarak döngüden çıkılır.
	int stop;			//Input dosyası içerisinde dosyanın sonuna gelinip gelinmediğini kontrol etmek amacıyla yazılmıştır. dosyanın sonuna gelinmişse stop = EOF olur.
	int tmpid;			//Input dosyası içerisindeki kişilerin id'si okunup bu değişkene yazılır.
	do{
		stop = fscanf(ptrfile, "%d %[^\n]s", &tmpid, text);  	//dosya okunur.
		
		if (x == tmpid){	//x girilen id, tmpid ise dosyada okunan id'dir. Aynı ise kişi dosyada var demektir.
			sign = 0;			//sign=0 olması demek verilen id'ye sahip kişinin dosyada bulunduğu anlamına gelir. Döngüden çıkılmasını sağlar.
			*id = tmpid;
			int i = 1; 			// text[] char dizisi ',' ile başlar. Kişilerin isimlerini çekerken ilk virgülden kurtulmak için i 1'den başlatılır.
			int j = 0;					//text dizisinde ilk virgül ile ikinci virgül arası kişi ismidir. 
			while(text[i] != ',' && text[i] != '\0'){	//? 	//Bu dizi ile ismi name[] char dizisine yazarız.
				name[j] = text[i];			//dosyadaki isim 'name' stringine yazılır.
				i++;
				j++;
			}
			name[j] = '\0';					//isim alındıktan sonra name[] dizisinde en son indise NULL character koyulur.
			j=0;
			if (text[i] == ','){i++;}		//bu şart kişinin hiç arkadaşı olmaması durumunda kodun düzgün çalışması için yazılmıştır.	              			
			while(text[i] != '\0'){			//text[] dizisinin devamındaki kişi arkadaşları friends[] dizisine yazılır.
			
				if (text[i] != '-'){						
					if ( text[i+1] != '-' && text[i+1] != '\0'){		// iki basamaklı mı diye kontrol edilir.
						friends[j] = (text[i] - '0')*10 + (text[i+1] - '0');		//iki basamaklı ise
						j++;
						i++;							
					}
					else{
						friends[j] = (text[i] - '0');						//tek basamaklı ise
						j++;
					}
				}
				i++;		
			}
			friends[j] = 0;		//kişi arkadaşları friends[] dizisine yazıldıktan sonra tampon olsun diye sıradaki indise 0 konulur.
		}
	}while(sign == 1 && stop != EOF); 	//sign = 0 olması verilen id Input dosyasında birisiyle eşleşmiş, stop = EOF olması durumunda dosyanın sonuna gelinmiştir.
	fclose(ptrfile);	// dosya kapatılır.
	return sign;
}

Person *newPerson(int p_id){ 						//Verilen id ile yeni bir node oluşturulur. Veriler Input dosyasından okunup node içersine yazılır ve o node döndürülür.
	Person *newnode = (Person *)malloc(sizeof(Person));
	int sign = readFile(p_id, &newnode->id, newnode->name, newnode->mates);			
	if (sign == 1){									// sign = 1 olması Input dosyasında  verilen id'ye sahip kimsenin olmadığı anlamına gelir.
		free (newnode);								
		printf("there is no such person in Input File \n");
		return;
	}
	newnode->left = NULL;
	newnode->right = NULL;
	return newnode;
}

Person *minIdPerson(Person *node){			//verilen nodu'un alt ağacında en küçük id'ye sahip node'u döndürrür.
		if (node == NULL){
			return NULL;
		}
		else if(node->left != NULL){
			return minIdPerson(node->left);
		}
		return node;
}

Person *insertNewUser(Person *node,int idnew){			//verilen id sahibi kişi ağaca yerleştirilir.
	if (node == NULL){
		return newPerson(idnew);													
	}
	else if (idnew < node->id){
		node->left = insertNewUser(node->left,idnew);
	}
	else if (idnew > node->id){
		node->right = insertNewUser(node->right,idnew);
	}
	return node;
}

Person *deleteUser(Person *node,int x){				//verilen id sahibi kişi ağaçtan silinir.
	if(node == NULL) {
		printf("that person have not been found \n");
		return NULL;
	}
	if (x < node->id){
		node->left = deleteUser(node->left, x);
	}
	else if (x > node->id){
		node->right = deleteUser(node->right, x);
	}
	else{
		if(node->left == NULL && node->right == NULL){		// silinecek node'un hiç çocuğunun olmaması durumu.
			free (node);
			return NULL;
		}
		else if(node->left == NULL && node->right != NULL){			//silinecek node'un sadece sağında çocuğunun olması durumu.
			Person *tmpnode = node->right;
			free(node);
			return tmpnode;
		}
		else if(node->left != NULL && node->right == NULL){			//silinecek nodu'un sadece solunda çocuğunun olması durumu.
			Person *tmpnode = node->left;
			free(node);
			return tmpnode;
		}
		else{
			Person *tmpnode = minIdPerson(node->right);				//silinecek nodu'Un hem sağında hem solunda çocuğunun olması.
			node->id = tmpnode->id;	
			strcpy(node->name, tmpnode->name);						
			int i = 0;
			while(tmpnode->mates[i] != 0){
				node->mates[i] = tmpnode->mates[i];
				i++;
			}
			node->right = deleteUser(node->right, tmpnode->id);
		}
	}
	return node;
}

void contains(Person *node, int x){				//id'si verilen kişiye ağaç içinde arayıp ekrana ismini basar.
	if(node == NULL) {printf("that person have not been found \n");}
	else{
		printf("compare %d with %d \n",x,node->id);
		if(x == node->id){
			printf(" %s \n", node->name);
		}
		else if(x < node->id){contains(node->left,x);}
		else if(x > node->id){contains(node->right,x);}
	}
}

Person *containsV2(Person *node, int x){ 	//friends fonksiyonunun içinde kullanılmak için yazılmıştır.
	if(node == NULL) {return;}				// contains() fonksiyonundan farkı eğer aranan elemanı ağaçta bulamazsa
	else{									// ekrana bir şey bastırmaz. node döndürür. 
		if(x == node->id){
			printf(" %s \n", node->name);
			return node;
		}
		else if(x < node->id){containsV2(node->left,x);}
		else if(x > node->id){containsV2(node->right,x);}
	}
}
void friends(Person *node, int x){			//id'si verilen kişinin arkadaşlarının isimlerini ekrana basar.
	Person *tmpnode;
	printf("\nfriends of ");
	tmpnode = containsV2(node,x);
	int i = 0;				//döngü ile arkadaş dizisi içinde dolaşmak için tanımlanmıştır.
	while(tmpnode->mates[i] != 0){
		containsV2(node,tmpnode->mates[i]);
		i++;
	}
}

void innerSize(Person *node, int *count){		// size() fonksiyonu icerisinde cagırılır.
	if (node != NULL){
		*count +=1;			//size() fonksiyonu içinde tanımlanmıştır. kişi sayısı tutulur.
		innerSize(node->left,count);
		innerSize(node->right,count);
	}
}
void size(Person *node){			// ağacın kaç kişiden oluştuğunu ekrana basar.
	int count = 0;					// kişi sayısının sayıldığı değişken.
	innerSize(node, &count);
	printf("Size of the tree is : %d \n",count);
}

void inOrderName(Person *node, int x){       		//printNext() fonksiyonu içinde çağırılır.
 	if(node != NULL){								//printInOrder() fonksiyonundan farkı yalnızca isimleri ekrana bastırmasıdır.
 		inOrderName(node->left,x);
		if ( x != node->id)							// girilen id değerine sahip kişinin ismini döndürmesin diye bu şart konuldu.
			printf("name : %s \n", node->name);
		inOrderName(node->right,x);
	}
}

void printNext(Person *node, int x){					//id'si verilen kişinin alt ağacındaki kişilerin isimlerini id leri küçükten büyüğe olacak şekilde ekarna basar.
	if(node == NULL) {printf("that person have not been found \n");}
	else{
		if(x == node->id){
			inOrderName(node,x);		
		}
		else if(x < node->id){printNext(node->left,x);}
		else if(x > node->id){printNext(node->right,x);}
	}
}

void printGreater(Person *node,int x){		//verilen id'den büyük id'si olan kişilerin id ve isimlerini ekrana bastırır.
	if(node != NULL){
		printGreater(node->left, x);
		if (x < node->id){
			printf("ID number : %d ", node->id);
			printf("name : %s \n",node->name);
		}
		printGreater(node->right, x);
	}
}

void printInOrder(Person *node){			//ağaçtaki kişiilerin isim ve idlerini inorder şeklinde ekrana bastırır.
	if(node != NULL){
		printInOrder(node->left);
		printf("ID number : %d\t ", node->id);
		printf("  name : %s \n",node->name);
		printInOrder(node->right);
	}
}


int main(){
	Person *tree = NULL;	//ağaç oluşturulur
	char choice;			//swich case yapısında case leri tutmak için kullanılmıştır.
	int idx;				//switch case yapısında verilen id leri tutmak için kullanılmıştır.
	printf("press a to insert a user\n");
	printf("press b to delete a user\n");
	printf("press c to learn if a user in the tree or not.\n");
	printf("press d to learn names of friends of a user\n");
	printf("press e to learn there are how many users in the tree\n");
	printf("press f to see subtree of a user by their id from small to large \n");
	printf("press g to see users whose have bigger id than a given user in the tree\n");
	printf("press h to see all users' names and ids by their id from small to large \n");
	printf("press x to exit program \n");
	printf("enter an operation : \n");
	while (1){
		scanf("%c",&choice);	

		switch(choice){
			case'a':
				printf("enter an id : ");
				scanf("%d",&idx);
				tree = insertNewUser(tree, idx);
				break;
				
			case 'b':
				printf("enter an id : ");
				scanf("%d",&idx);
				tree = deleteUser(tree, idx);
				break;
				
			case 'c':
				printf("enter an id : ");
				scanf("%d",&idx);
				contains(tree, idx);
				break;
			
			case 'd':
				printf("enter an id : ");
				scanf("%d",&idx);
				friends(tree, idx);
				break;
			
			case 'e':
				size(tree);
				break;
				
			case 'f':
				printf("enter an id : ");
				scanf("%d",&idx);
				printNext(tree, idx);
				break;
				
			case 'g':
				printf("enter an id : ");
				scanf("%d",&idx);
				printGreater(tree, idx);
				break;
				
			case 'h':
				printInOrder(tree);
				break;
				
			case 'x':
				exit(0);
				break;
			default:
				break;
		}		
	}
	return 0;
}
