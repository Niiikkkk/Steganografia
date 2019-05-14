#include "class.hpp"

int main(){  /*main Tester della classe Handler*/
	Byte_Handler* byte = new Byte_Handler();

	int r = byte -> getBit('c' , 6);  /*01100011 = c*/

	printf("%d\n",r);


	char c = byte -> changeBit('c' , 3 , 1);
	printf("%c\n",c);

	char z = byte -> compareBitsLSB('c' , 'g' , 2);
	printf("%c\n",z);
	printf("%d\n",byte->getBit(z,1));

}