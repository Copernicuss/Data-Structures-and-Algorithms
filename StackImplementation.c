#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define SIZE 20

int stackoperand[SIZE];             
char stackoperator[SIZE];
int topoperand = 0;          //Stack boş iken top değişkenleri 0'ı gösterir.
int topoperator = 0;

int checkState(int top){			//Stack dolu ise 1, boş ise -1 döndürür.
	if (top == SIZE)				//Diğer durumlarda ise 0 döndürür.
		return 1;
	else if (top == 0)
		return -1;
	else
		return 0;
}
void printStacks(){               // Stacklerin içerisindeki operand ve operatorleri ekrana bastırır.
	int i;
	printf("\nOperand Stack  : ");
	for(i=0;i<topoperand;i++){
		printf("%d   ",stackoperand[i]);
			
	}
	printf("\nOperator Stack  : ");
	for(i=0; i<topoperator; i++){
		printf("%c  ",stackoperator[i]);
	}	
}


void pushOperand(int item){									// Operand stack'ine eleman gönderip, stack'de o anda bulunan elemanları ekrana basar.
	if (checkState(topoperand) == 1 )
		printf("Operand Stack is Full \n");
	else
		stackoperand[topoperand] = item;
		topoperand += 1;
		printStacks();
}
void pushOperator(char item){								// Operator stack'ine eleman gönderip, stack'de o anda bulunan elemanları ekrana basar.
	if (checkState(topoperator) == 1 )
		printf("Operator Stack is Full \n");
	else
		stackoperator[topoperator] = item;
		topoperator += 1;
		printStacks();
}

int popOperand(){										// Operand stack'inin en üst elemanını çıkarıp döndürür.
	int rtn;
	if (checkState(topoperand) == -1)
		printf("Operand Stack is Empty \n");
	else 
		rtn = stackoperand[topoperand-1];
		topoperand -= 1;
		return rtn;
}
char popOperator(){											// Operator stack'inin en üst elemanını çıkarıp döndürür.
	char rtn;
	if (checkState(topoperator) == -1)
		printf("Operator Stack is Empty \n");
	else 
		rtn = stackoperator[topoperator-1];
		topoperator -= 1;
		return rtn;
}

int peakOperand(){ 											// Operand stack' inin tepesindeki elemanı döndürür.
	if (topoperand == 0)
		return;
		//printf("Empty Operand \n");
	else
		return stackoperand[topoperand-1];
}
char peakOperator(){									// Operatör stack' inin tepesindeki elemanı döndürür.
	if (topoperator == 0){
		//printf("Empty Operator\n");
		return '?';
	}
	else
		return stackoperator[topoperator-1];
}

int isPrec(char op){						// Operatörlerin önceliğini kıyasladığımız fonksiyon.
	if (op == '(' || op == ')' )
		return 999;
	else if (op == '*' || op == '/')
		return 99;
	else if (op == '+' || op == '-')
		return 9;
	else
		return 0;
}

int opFunc(int x,int y,char op){        	// İki sayı ile operatörü işleme sokan fonksiyon
	if (op == '+') {return (x+y);}
	else if (op == '-'){return (x-y);}
	else if (op == '*'){return (x*y);}
	else if (op == '/'){return (x/y);}
	else {return 0;}
}

int intConcat(int m, int n) {						// 10'dan büyük sayıları okumak için iki int değerini concatenate eder.
    return (10*m + n);
}

char delSpaces(char *str){             // input da okunan string içindeki boşlukları atan fonksiyon.
	int i,yer = 0;
	
	for(i=0; str[i]; i++){
		if (str[i] != ' '){
			str[yer] = str[i];
			yer += 1;
		}
	}
	str[yer] = '\0';
	return *str;
}

int convertFunc(char c){           //Karakter dizisini integer'a dönüştürür.
	int dec;
	dec = c-'0';
	return (dec);
}

int main() {	
	
	char inpt[30];	 							// Kullanıcıdan string okur. Okunan string'in içideki boşlukları siler.
	gets(inpt);
	delSpaces(inpt);
	
	
	int i = 0,value;
	while (inpt[i] != '\0'){
		int convert1,convert2,control = 100;

		if (isdigit(inpt[i])){							// dizinin o anki elemanı sayı olup olmadığını kontrol eder.
			if (isdigit(inpt[i+1])){						// Eğer sayı ise bir sonraki elemanının da sayı olup olmadığına bakar. 
				convert1 = convertFunc(inpt[i]);
				convert2 = convertFunc(inpt[i+1]);						
				value = intConcat(convert1, convert2);
				pushOperand(value);
				i++;
			}
			else{
				convert1 = convertFunc(inpt[i]);
				value = convert1;					
				pushOperand(value);
			}
		}
			
			
		else if ( inpt[i] == '(')								// sol parantez ise operator stack' ine atar.
			pushOperator(inpt[i]);
			
		else if (inpt[i] == ')')								//sağ parantez ise sol parantezi görene kadar işlemleri yapar.
			while(control == 100){    
				char ab = popOperator();
				int a = popOperand();
				int b = popOperand();
				int	c = opFunc(b,a,ab);
				pushOperand(c);
				if (peakOperator() == '(')						//sol parantez stack'in en üstüne geldiğinde dışarı atılır. Ve döngüden çıkılır.
					popOperator();
					control = 101;
			}
			
						
		else
			if (peakOperator() == '('){						// en üstte sol parantez varsa, gelen işareti stack'a atar.
				pushOperator(inpt[i]);
			}
			else if (isPrec(peakOperator()) < isPrec(inpt[i]))		//gelen işaretin önceliği stack'in en üstündeki işaretin önceliğinden büyükse
				pushOperator(inpt[i]);								// veya stack içerisi boş ise gelen işareti stack'a atar.
				
			else if	(isPrec(peakOperator()) >= isPrec(inpt[i])){			//gelen işaretin önceliği stack'in en üstündeki işaretin önceliğinden küçükse...
				char ab = popOperator();
				int a = popOperand();
				int b = popOperand();
				int	c = opFunc(b,a,ab);
				pushOperand(c);
				pushOperator(inpt[i]);
				
			}
		i++;
	}
	while(topoperand !=0 && topoperator != 0){				
								
		char ab = popOperator();		
		int a = popOperand();
		int b = popOperand();
		int	c = opFunc(b,a,ab);
		pushOperand(c);
				
	}
	printf("\n\nThe result is  :%d \n",popOperand());
	return 0;
}
