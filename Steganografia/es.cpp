#include "class.hpp"

int main(){  /*main Tester della classe Handler*/
	Byte_Handler* byte = new Byte_Handler();

	int r = byte -> getBit('c' , 1);  /*01100011 = c*/

	printf("%d\n",r);


	char c = byte -> changeBit('c' , 1 , 0);
	printf("%c\n",c);

	char z = byte -> compareBitsLSB('c' , '+' , 1);
	printf("%c\n",z);


	FILE* text = fopen("testo.txt");
	int cd = byte -> generateKey(text);
	printf("%d\n",cd);
}