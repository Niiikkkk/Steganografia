#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>


class ByteHandler{
	public:
		int getBit(char , int ); /*return 1 or 0*/
		char changeBit(char  , int , int );
		char compareBitsLSB(char , char  , int );
		int generateKey(FILE*);
		int getLength(FILE*);
		int getImageDataOffset(FILE*);
	private:
		/*PASS*/
};



int ByteHandler::getBit(char byte, int pos){  /*return 1 or 0 , sifta di 8-pos(1 a 8) a sinistra e poi lo riscifta verso descra , così da togliere tutti i bit a sinistra e poi a destra e poi fa AND con 1
																la POS VA DA 1 A 8 , dove 1 è quello più a destra!!!!!!!!!!!*/
	int bit = (byte<<8-pos);
	bit = bit >> 7 & 1;
	return bit;
}


char ByteHandler::changeBit(char byte ,int pos, int flag){  /*prende il byte , controlla cosa c'è in quella pos e se è diverso da flag lo cambia di tow[pos-1], ho creato two , la potenza del 2 fino a 7, cosi 
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

char ByteHandler::compareBitsLSB(char first, char second , int pos_first){  /*mette nell'ultimo bit della seconda il pos_first bit dell' first*/
	char buff_second = second;
	int bit_first = getBit(first , pos_first);  /*8-pos perchè se faccio un for lui va da 0 a n , quindi 8 - pos*/
	int bit_second = getBit(second , 1);
	if(bit_second != bit_first){
		buff_second = changeBit(second , 1 , bit_first);
	}
	return buff_second;
}

int ByteHandler::generateKey(FILE* text){   /*restituisce la chiave */
	int length = getLength(text);
	srand(time(NULL));
	return (rand() % 100);
}

int ByteHandler::getLength(FILE *fp) {
	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	return(size);
}

int ByteHandler::getImageDataOffset(FILE* bmp_offset) {
	fseek(bmp_offset,10,SEEK_SET); /*in 10 c'è l'offset*/
	int offset;
	offset=(int)fgetc(bmp_offset);
	return offset;
}