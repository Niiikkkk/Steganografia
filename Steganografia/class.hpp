#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>


class Byte_Handler{
	public:
		int get_Bit(char , int ); /*return 1 or 0*/
		char change_Bit(char  , int , int );
		char compare_Bits_LSB(char , char  , int );
		int generate_Key(FILE*);
		int get_Length(FILE*);
		int get_Image_Data_Offset(FILE*);
	private:
		/*PASS*/
};



int Byte_Handler::get_Bit(char byte, int pos){  /*return 1 or 0 , sifta di 8-pos(1 a 8) a sinistra e poi lo riscifta verso descra , così da togliere tutti i bit a sinistra e poi a destra e poi fa AND con 1
																la POS VA DA 1 A 8 , dove 1 è quello più a destra!!!!!!!!!!!*/
	int bit = (byte<<8-pos);
	bit = bit >> 7 & 1;
	return bit;
}


char Byte_Handler::change_Bit(char byte ,int pos, int flag){  /*prende il byte , controlla cosa c'è in quella pos e se è diverso da flag lo cambia di tow[pos-1], ho creato two , la potenza del 2 fino a 7, cosi 
																			a seconda della pos aggiungo o tolgo il numero giusto*/
	int two[] = {1,2,4,8,16,32,64,128};
	char mod_byte = byte;
	if(get_Bit(byte,pos) == 0 && flag == 1){
		mod_byte = byte + two[pos-1];
	}
	if(get_Bit(byte,pos) == 1 && flag == 0){
		mod_byte = byte - two[pos-1];
	}
	return mod_byte;
}

char Byte_Handler::compare_Bits_LSB(char first, char second , int pos_first){  /*mette nell'ultimo bit della seconda il primo bit dell' first*/
	char buff_second = second;
	int bit_second = get_Bit(second , 1);
	int bit_first = get_Bit(first , 8 - pos_first);  /*8-pos perchè se faccio un for lui va da 0 a n , quindi 8 - pos*/
	if(bit_second != bit_first){
		buff_second = change_Bit(second , 1 , bit_first);
	}
	return buff_second;
}

int Byte_Handler::generate_Key(FILE* text){   /*restituisce la chiave */
	int length = get_Length(text);
	srand(time(NULL));
	return (rand() % 100);
}

int Byte_Handler::get_Length(FILE *fp) {
	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	return(size);
}

int Byte_Handler::get_Image_Data_Offset(FILE* bmp_offset) {
	fseek(bmp_offset,10,SEEK_SET); /*in 10 c'è l'offset*/
	int offset;
	offset=(int)fgetc(bmp_offset);
	return offset;
}