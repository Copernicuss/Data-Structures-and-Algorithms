#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define SIZE 6

void printArrFunc(int ar[]){		//verilen diziyi ekrana basar.
	int i;
	for(i=0; i<SIZE; i++){
		printf("%d  ", ar[i]);
	}
}
int randFunc(int l,int r){			//verilen iki sayının arasında rastgele bir sayı üreten fonksiyon.
	srand(time(0));
	int rdm = l + (rand() % (r-l) +1);
	return rdm;
}
void swapFunc(int *first_p,int *second_p){		//verilen iki adresteki degerleri degistirir.
	int tmp;
	tmp = *first_p;
	*first_p = *second_p;
	*second_p = tmp; 
}

int partFunc(int l,int r,int pivot, int arr[]){ 		// l=i dizinin başlangıcı, r=j dizisini sonu
	int i=l;											//bu fonksiyon verilen diziyi verilen pivottan küçükler ve büyükler şeklinde sıralar.
	int j=r;
	while(i < j){ 								
        while(i < j && arr[i] < pivot) {i++;}		//pivottan küçük olduğu sürece döner
        while(i < j && arr[j] > pivot) {j--;}		// pivottan büyük olduğu sürece döner.	
   	swapFunc(&arr[j], &arr[i]);					
	}
    return j;				// sıralama sonrası pivotun olduğu indistir.
}

void qsFunc(int l,int r,int lock[],int key[]){		// l dizinin başlangıcı, r dizisini sonu
	if (l < r){
		int rdm = randFunc(l,r);					//key dizisinde rastgele bir eleman seçer.
		int pivot = key[rdm];
		
		printf("pivot : %d   ",pivot);
		int x = partFunc(l,r,pivot,lock);		//key dizisinden rastgele seçilen pivota göre lock düzenlenir.lock dizisindeki o elemanın adresi döndürülür. (x) <---
		
		printf(" Lock Array :  ");
		printArrFunc(lock);					
		
		partFunc(l,r,lock[x],key);				//key dizisi lock[x]'e göre sıralanır.
		printf("	Key  Array : ");
		printArrFunc(key);

		printf("\n");
		qsFunc(l,x-1,lock,key);							//x'in sol tarafı
		qsFunc(x+1,r,lock,key);							//x'in sağ tardfı
	}
}


int main(){
	
	//int lock[SIZE] = {6,4,5,1,3,2};
	//int lock[SIZE] = {1,2,3,4,5,6};		//örnek diziler.
	//int lock[SIZE] = {6,5,4,3,2,1};
	//int key[SIZE]  = {4,3,5,6,1,2};
	//int key[SIZE] = {1,2,3,4,5,6};
	//int key[SIZE] = {6,5,4,3,2,1};
	
	
	printf(" enter  %d elements. \n", SIZE); 
	int i;
	int lock[SIZE],key[SIZE];
	printf("enter the locks\n");
	for(i=0;i<SIZE;i++){
		scanf("%d", &lock[i]);
	}
	printf("enter the keys\n");
	for(i=0;i<SIZE;i++){
		scanf("%d", &key[i]);
	}
	
	printf("Lock Array : ");
	printArrFunc(lock);

	printf("\t");
	
	printf("Key  Array : ");
	printArrFunc(key);

	printf("\n*************************************************\n");
	
	qsFunc(0, SIZE-1, lock, key);
	printf("\n\n THE RESULT ********************************** \n");
	
	printf("Lock Array : ");
	printArrFunc(lock);

	printf("\t");
	printf("Key  Array : ");
	printArrFunc(key);
	
	return 0;
}
