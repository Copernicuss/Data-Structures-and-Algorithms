#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define SIZE 20

typedef struct{		//noktalar struct kullanılarak gerçekleştirilmiştir.
	int x;
	int y;	
}point;

void createPoint(int x, int y,int *numPoint, point *plane2D[]){			//readFile() içinde çalışır. Dosyadan okunan noktaları oluşturup plane2D isimli diziye ekler.
	point *newPoint = (point*)malloc(sizeof(point));
	newPoint->x = x;
	newPoint->y = y;
	plane2D[*numPoint] = newPoint;			//plane2D problemdeki uzayımızdır.
	(*numPoint)++;							//numPoint ile kaç tane nokta olduğunu tutarız.
}

void readFile(point* p[], int *n){		// dosyadan noktalar okunup oluşturulur.
	FILE *ptrfile;
	ptrfile = fopen("ornek.txt", "r");
	if (ptrfile == NULL){
		printf("no file is found.");
		return;
	}
	int x,y;
	while(fscanf(ptrfile, "%d %d", &x, &y) != EOF){
		createPoint(x,y,n,p);
	}
	fclose(ptrfile);
}
int partFunc(int l, int r, point* p[],char ch) //quicksort algoritmasının partition kısmı. ch='y' olursa noktaları y koordinatlarına göre sıralar.
{ 	
	int pivot;
	if (ch == 'y') {pivot = p[r]->y;}
    else{ pivot = p[r]->x; }   
    int i = (l-1);   
	int j;
	point* tmp;
    for (j=l; j<=r-1;j++) { 
    	if (ch=='y'){
    		if (p[j]->y < pivot ){
    			i++; 
            	tmp = p[j];
				p[j] = p[i];
				p[i] = tmp;
			}
		}
		else{
        	if (p[j]->x  < pivot) { 
            	i++; 
            	tmp = p[j];
				p[j] = p[i];
				p[i] = tmp; 
			}
        } 
    }
	tmp = p[r];
	p[r] = p[i+1];
	p[i+1] = tmp;  
    return (i + 1); 
} 

void qsFunc(int l,int r, point* p[], char ch){ // ch='y' olursa noktaları y koordinatlarına göre sıralar.
	if (l < r){
		int s = partFunc(l,r,p, ch); 
		qsFunc(l,s-1,p,ch);							
		qsFunc(s+1,r,p,ch);															
	}
}

float findDistance(point* p1, point* p2){		//iki nokta arası uzaklığı bulup döndürür.
	return sqrt( (p1->x - p2->x)*(p1->x - p2->x) + (p1->y - p2->y)*(p1->y - p2->y) );
}

float bruteForce(point* p[], int l, int r, point* closestPoint[]){	// Sağ veya sol tarafta 2-3 nokta kaldığında çağrılır. Aralarındaki minimum mesafe bulunur. recursiveFunc() içinde çağırılır.
	int i,j;
	float min = 1000;  //rastgele, büyük bir sayı yazılmıştır.
	for(i=l; i<r+1 ; i++){
		for(j=i+1 ; j<r+1 ; j++){
			if(findDistance(p[i], p[j]) < min)  {
				min = findDistance(p[i], p[j]);
				if(findDistance(p[i], p[j]) < findDistance(closestPoint[0], closestPoint[1])){
					closestPoint[0] = p[i];  
					closestPoint[1] = p[j];		// burada birbirine en yakın iki noktanın hangisi olduğu tutulur.
				}
			}
		}			
	}
	return min;
}

float compareDistance(float f1, float f2){	//iki mesafeyi kıyaslayıp küçük olanı döndürür.
	if (f1 > f2) {return f2;}
	else {return f1;}
}

float func(int r,float dmin, point* nearPoints[], point* closestPoint[]){	//recursiveFunc içinde çağrılır. median değerinden x'e göre dmin uzaklığındaki noktaları y'ye göre sıralayıp 
	float min = dmin;														//aralarındaki en kısa mesafeyi bulur.
	qsFunc(0,r,nearPoints, 'y');			//y'ye göre sıralama
	int i,j;
	for(i=0; i<r+1;i++){
		for(j=i+1; j<r+1; j++){
			if((nearPoints[j]->y - nearPoints[i]->y < min) && (findDistance(nearPoints[i], nearPoints[j]) < min) ){	
				min = findDistance(nearPoints[i], nearPoints[j]); 
				if((findDistance(nearPoints[i], nearPoints[j])) < (findDistance(closestPoint[0], closestPoint[1]))){
					closestPoint[0] = nearPoints[i];  
					closestPoint[1] = nearPoints[j];		// burada birbirine en yakın iki noktanın hangisi olduğu tutulur.
				}
			}
		}
	}
	return min;
}

float recursiveFunc(point* p[],int l, int r, point* closestPoint[]){// medyan değerine göre ekseni sürekli ikiye böler.  //clasestp en yakın iki noktayı tutar.
	int n = r-l+1;	//eleman sayısı
	if (n <= 3 ){
		return bruteForce(p,l,r, closestPoint);		//n 2-3 durumunda bruteForce() çağrılır.
	}
	int middle = (l+r)/2;	//median bulunur.
	
	float lmin = recursiveFunc(p,l,middle,closestPoint);		//sol bölgedeki minimum
	float rmin = recursiveFunc(p,middle+1,r,closestPoint);		//sağ bölgedeki minimum
	float dmin = compareDistance(lmin,rmin);
	
	int i=1; 
	int j=0;
	point* nearPoints[SIZE];	//median değerine x eksenine göre 'dmin' uzaklığından daha yakın olan noktalar ayrı bir dizi içerisine konur.
	while((middle-i >=l) && (p[middle]->x - p[middle-i]->x < dmin)){ //median ın solundakiler
		nearPoints[j] = p[middle-i];
		i++;
		j++;
	}
	i = 1;
	nearPoints[j] = p[middle];
	j++;
	while((middle+i <=r) && (p[middle+i]->x - p[middle]->x < dmin)){	//median ın sağındakiler
		nearPoints[j] = p[middle+i];
		i++;
		j++;
	}
	float dmin2 = func(j-1, dmin, nearPoints,closestPoint);
	return dmin2;
}


int main(){
	
	point *plane2D[SIZE];		// 2 boyutlu uzay.
	int numPoint = 0; 			//dosyadan kaç tane nokta okunup oluşturulduğu bilgisi.
	point *closestPoint[2];		//birbirine en yakın iki noktayı tutar.
	readFile(plane2D, &numPoint);
	
	int i;
	for(i=0;i<numPoint;i++){
		printf("x :%d   y:%d \n", plane2D[i]->x, plane2D[i]->y);
	}
	closestPoint[0] = plane2D[0];
	closestPoint[1] = plane2D[1];
	
	qsFunc(0, numPoint-1, plane2D,'x');
	
	float result = recursiveFunc(plane2D,0,numPoint-1, closestPoint);
	
	printf("\npoint1 (x,y): (%d,%d) \n", closestPoint[0]->x, closestPoint[0]->y);
	printf("point2 (x,y): (%d,%d) \n", closestPoint[1]->x, closestPoint[1]->y);
	printf("Distance between the two points: %f \n", result);
	
	return 0;
}

