#include "ByteHandler.hpp"

int main(){  /*main Tester della classe Handler*/
	ByteHandler* byte = new ByteHandler();

	int r = byte -> getBit('c' , 6);  /*01100011 = c*/

	printf("%d\n",r);


	char c = byte -> changeBit('c' , 1 , 0);
	printf("%c\n",c);

	char z = byte -> compareBitsLSB('c' , 'g' , 2);
	printf("%c\n",z);
	printf("%d\n",byte->getBit(z,1));

}
