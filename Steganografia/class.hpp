#include <stdio.h>
#include <stdlib.h>
#include <math.h>

class Byte_Handler{
	public:
		int getBit(char byte , int pos); /*return 1 or 0*/
		char changeBit(char byte , int pos, int flag);
		char compareBitsLSB(char first, char second , int pos_first);
	private:
		/*PASS*/
};



int Byte_Handler::getBit(char byte, int pos){  /*return 1 or 0 , sifta di 8-pos(1 a 8) a sinistra e poi lo riscifta verso descra , così da togliere tutti i bit a sinistra e poi a destra e poi fa AND con 1
																la POS VA DA 1 A 8 , dove 1 è quello più a destra!!!!!!!!!!!*/
	int bit = (byte<<8-pos);
	bit = bit >> 7 & 1;
	return bit;
}


char Byte_Handler::changeBit(char byte ,int pos, int flag){  /*prende il byte , controlla cosa c'è in quella pos e se è diverso da flag lo cambia di tow[pos-1], ho creato two , la potenza del 2 fino a 7, cosi 
																			a seconda della pos aggiungo o tolgo il numero giusto*/
	int two[] = {1,2,4,8,16,32,64,128};
	char mod_byte = byte;
	if(getBit(byte,pos) == 0 && flag == 1){
		mod_byte = byte + two[pos-1];
	}
	if(getBit(byte,pos) == 1 && flag == 0){
		mod_byte = byte - two[pos-1];
	}
	return mod_byte;
}

char Byte_Handler::compareBitsLSB(char first, char second , int pos_first){  /*mette nell'ultimo bit della seconda il primo bit dell' first*/
	char buff_second = second;
	int bit_second = getBit(second , 1);
	int bit_first = getBit(first , 8 - pos_first);  /*8-pos perchè se faccio un for lui va da 0 a n , quindi 8 - pos*/
	if(bit_second != bit_first){
		buff_second = changeBit(second , 1 , bit_first);
	}
	return buff_second;
}