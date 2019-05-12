#include "class.hpp"

int main(){  /*main Tester della classe Handler*/
	Byte_Handler* byte = new Byte_Handler();

	int r = byte -> getBit('c' , 3);  /*01100011 = c*/

	printf("%d\n",r);


	char c = byte -> changeBit('c' , 1 , 0);
	printf("%c\n",c);

	char z = byte -> compareBitsLSB('c' , '+' , 3);
	printf("%c\n",z);

}