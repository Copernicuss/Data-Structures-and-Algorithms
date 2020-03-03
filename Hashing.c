#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <math.h>
#include <string.h>
#define SIZE 250
#define M 11
#define MM M-1
#define R 3

char *hashTable[M];

long long int h1(long long int key){	//hash fonksiyonları
	return (key % M);
}
long long int h2(long long int key){	//hash fonkisyonları
	return 1 + (key % MM);
}
long long int h(long long int key, int i){		//hash fonksiyonları
	return (h1(key) + i*h2(key))%M;
}

void insertHash(char* str,long long int key){	//verilen dosya ismini, key'den hash value üreterek  tabloya koyar.
	int flag=0;	// döngüde bayrak olarak kullanılır.
	int i=0;		//double hashing formulunde kullanılır.
	int h_sayi;
	while(!flag){
		h_sayi = h(key,i);
		printf("yerlestirilecek indeks : %d \n", h_sayi);
		if(hashTable[h_sayi] == NULL){
			printf("yerlestirilen indeks : %d \n\n\n", h_sayi);		
			hashTable[h_sayi] = strdup(str);
			flag = 1;
		}
		else {i++;}
	}
}

long long int stringHashFunc(char *str){		//verilen satırı(kelime kelime) hash fonksiyonundan geçirir.
	int i=0,j=0,left=0;
	long long int  key_sum=0;
	long long zz= 0;
	if(!str[0]) {return 0;}			    //dosya boş ise 0 döndürür.
	while(str[i] && str[i] != '\n'){				
		if(str[i] == ' ' || str[i+1] == '\n' || (!str[i+1])){
			while(str[i] == ' ' && str[i+1] != '\n' && (str[i+1])){i++;}
			if(str[i+1] == '\n' || (!str[i+1])) {i++;}
			for (j = left; j < i; j++){
				zz = str[j] * pow(R, (i-j));
				key_sum = key_sum + zz;
			}
			left = i;
			i--;
		}
		i++;
	}
	return key_sum;
}

void readFile(){	
	FILE *ptrfile;
	FILE *ptrfile_2;
	ptrfile = fopen("samples.txt", "r");
	if (ptrfile == NULL)  {printf("no file is found. (readFile()-1) \n");  return; }
	char oneLine[SIZE];		//okunan satırlar bu dizilere atılır.
	char oneLine_2[SIZE];	
	int i=0;		
	long long int sumAll=0;
	long long int z=0;
	int lengthS;
	while( fgets(oneLine, SIZE, ptrfile) != NULL){
		printf(" %s \n", oneLine);
		lengthS = strlen(oneLine);				//sample.txt dosyası içindeki dosya isimleri parametre olabilecek şekilde düzenlenir. 
		char *buffer;	
		buffer = strdup(oneLine);
		buffer[lengthS-1] = '\0'; 	
	
		ptrfile_2 = fopen(buffer, "r");	
		if (ptrfile_2 == NULL) { printf("no file is found. readFile()-2 \n" );  return; }
		
		while(fgets(oneLine_2, SIZE, ptrfile_2) != NULL){			//dosyanın her satırı açılıp hash fonksiyonundan geçirilir.
			z = stringHashFunc(oneLine_2);
			//printf("z : %lld ",z);
			sumAll = sumAll + z;
			memset(oneLine_2, 0, sizeof(oneLine_2));
		}
		//printf("sumAll : %lld \n", sumAll);
		fclose(ptrfile_2);
		insertHash(oneLine,sumAll);						//dosya ismi ve key bu fonksiyona gönderilir. Fonksiyon hashTable 'a ekleme yapar.
		sumAll = 0;
		z = 0;
		memset(oneLine, 0, sizeof(oneLine));
	}
	fclose(ptrfile);
}

void readNewFile(char* aFile){
	FILE *ptrfile_3;	
	FILE *ptrfile_4;
	FILE *ptrfile_5;
	char oneLine_3[SIZE];			//okunan satırlar bu dizilere atılır.
	long long int key=0;			//hash sayısı hesaplamada kullanılır.
	long long int z=0;				//hash sayısı hesaplamada kullanılır.
	ptrfile_3 = fopen(aFile, "r");
	if (ptrfile_3 == NULL)  { printf("no file is found.((readNewFile()-1))\n" );  return; }
	
	while(fgets(oneLine_3, SIZE, ptrfile_3) != NULL){		//dosyanın her satırı açılıp hash fonksiyonundan geçirilir.
		z = stringHashFunc(oneLine_3);
		//printf("z : %lld", z);
		key = key + z;
		memset(oneLine_3, 0, sizeof(oneLine_3));
	}
	fclose(ptrfile_3);
	//printf("key : %lld \n", key);
	int flag=0;					//while döngüsünde bayrak olarak kullanılır. İşlem tamamlandığında 1 verilir ve döngüden çıkar.
	int i=0;					//h(key,i) double hashing fonksiyonunda kullanılır.
	int h_sayi;	
	while(!flag){
		h_sayi = h(key,i);
		printf("yerlestirme - indeks:%d \n", h_sayi);
		if(hashTable[h_sayi] == NULL){						//hash fonksiyonundan elde edilen hash sayısının tabloda gösterdiği yer boş ise
			hashTable[h_sayi] = strdup(aFile);				//oraya yazılır.
			flag = 1;
			printf("Son yerlestirme - indeks:%d \n", h_sayi);
			
			ptrfile_5 = fopen("samples.txt", "r+");					//samples.txt dosyasına dosya ismi yazılır.
			printf("samples.txt icine yaziliyor.... \n\n");
			fseek(ptrfile_5,0,SEEK_END);
			//fputs("\n",ptrfile_3);
			fputs(aFile, ptrfile_5);							//samples.txt adresine dokümanın ismi yazılacak.
			fputs("\n",ptrfile_5);
			fclose(ptrfile_5);
		}
		else{														//hash fonksiyonundan elde edilen hash sayısının tabloda gösterdiği yer boş değil ise
			char *tmp = strdup(hashTable[h_sayi]);					//o adresteki dosya açılır. İki dosyanın içerikleri kıyaslanır. 
			tmp[strlen(tmp)-1] = '\0';
			ptrfile_4 = fopen(tmp, "r");
			if (ptrfile_4 == NULL)  { printf("no file is found. (readNewFile()-2)\n");  return; }
			char oneLine_4[SIZE];
			ptrfile_3 = fopen(aFile, "r");
			if (ptrfile_3 == NULL) { printf("no file is found. (readNewFile()-3)\n");  return;
			}
			int isSame = 0;	//iki dosya icerigi kontrol edilirken bayrak olarak kullanılır.
			while(isSame == 0 && ((fgets(oneLine_3, SIZE, ptrfile_3) != NULL) && (fgets(oneLine_4, SIZE, ptrfile_4) != NULL))){	
				isSame = strcmp(oneLine_3, oneLine_4);															//içerikler farklı ise i bir artırılarak döngü devam eder.
				memset(oneLine_3, 0, sizeof(oneLine_3));	
				memset(oneLine_4, 0, sizeof(oneLine_4));
			}															//icerik bir noktaya kadar aynı ama biri daha fazla satıra sahip ise burada anlaşılır.
			if(((fgets(oneLine_3, SIZE, ptrfile_3) == NULL) && (fgets(oneLine_4, SIZE, ptrfile_4) != NULL)) || 		
					((fgets(oneLine_3, SIZE, ptrfile_3) != NULL) && (fgets(oneLine_4, SIZE, ptrfile_4) == NULL))){		//satır sayıları farklı oldugunda isSame = 2 olur.
				isSame = 2;								
				printf(" satir sayilari farkli. ! \n");
			}
			memset(oneLine_3, 0, sizeof(oneLine_3));
			memset(oneLine_4, 0, sizeof(oneLine_4));	
			if(isSame == 0) {flag = 1; printf(" Same file ! \n");}		
			else {i++;}
		}		
	}	
	fclose(ptrfile_4);
	fclose(ptrfile_3);
}


int main(){
	readFile();
	
	printf("********** yeni dosya yerlestirme ***********\n");
	readNewFile("yenidosya_2.txt");
	printf("********** yeni dosya yerlestirme ***********\n");
	readNewFile("yenidosya_3.txt");
	printf("********** yeni dosya yerlestirme ***********\n");
	readNewFile("yenidosya_xx.txt");
	
	int i;
	printf("\n");
	for(i=0;i<M;i++){
		printf("indeks %d : %s \n",i, hashTable[i]);
	}
	
	
	return 0;
}
