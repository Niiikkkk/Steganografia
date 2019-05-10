#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <array>
#include <vector>
#include <iterator>
#include <limits.h>
#include <math.h>
#define MAX_INT_POINTS 250

int* get_points_esp();

int* get_points_log();

int get_image_data_offset(FILE* bmp_offset) {
	fseek(bmp_offset,10,SEEK_SET); /*in 10 c'è l'offset*/
	int offset;
	offset=(int)fgetc(bmp_offset);
	return offset;
}

int get_message_length(FILE *fp) {
	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	return(size);
}
int get_bit(unsigned char the_byte,int which_bit) {
	return((the_byte>>8-which_bit)&1);    /*11110100  &  10000000   controlla se in 1 pos c'è 1*/
}

int main(int argc,char** argv) {
	FILE *first_image; //first_image
	FILE *text; //message_handle
	FILE *second_image; //hidden_message_handle

	if(argc!=5) {
		printf("Usage: %s [-e][-d] <source file> <destination file> <text file>\n-e : Add text to image\n -d : Get text from image\n",argv[0]);
		exit(1);
	}

	int mode;  /*setta la modalità a 1 se è in Encode o 0 se è in Decode*/
	if(strcmp(argv[1],"-e") == 0)
		mode=1;
	else if(strcmp(argv[1],"-d") == 0)
		mode=0;
	else {
		printf("Usage: %s [-e][-d] <source file> <destination file> <text file>\n-e : Add text to image\n -d : Get text from image\n",argv[0]);
		exit(1);
	}

	/* HANDLING FILE OPENING AND ERRORS */
	first_image=fopen(argv[2],"rb"); /*apre la prima immagine , l'originale in readbyte*/
	if (first_image == NULL) {
		fprintf(stderr, "Can't open input file %s\n",argv[2]); /*Messaggio di errore*/
		exit(1);
	}

	second_image=fopen(argv[3],"wb"); /*Apre la seconda immagine , quella su cui mettere la steganografia*/
	if (second_image== NULL) {
		fprintf(stderr, "Cannot create output file %s\n",argv[3]); /*Messaggio di errore*/
		exit(1);
	}

	int c=0;

	/* Generate file with the same header. Copy first 128 bytes */
	char tmp_sig_cpy;
	int offset=get_image_data_offset(first_image);
	printf("offset : %d\n",offset);

	rewind(first_image);

	int length_img =  get_message_length(first_image);

	for(int i=0;i<offset;i++) {  /*Copia l'header dell'immagine nell'altro file di immagine*/
		tmp_sig_cpy=fgetc(first_image);
		fputc(tmp_sig_cpy,second_image);
	}
	/* Made file as .bmp */

	char file_buffer;			// Temp variable for one byte from file
	char message_buffer;		// Temp buffer for one byte of message
	

	if(mode == 1) {  /*fgetc prende il carattere , il byte*/
		printf("Encode\n");
		FILE* text = fopen(argv[4],"r");
		if(text == NULL){
			printf("Error during opening file\n");
			exit(-1);
		}
		int max_first = get_message_length(first_image);
		int* points = get_points_esp();
		int max_text = get_message_length(text);
		char buff_image;
		char buff_text;
		fseek(first_image,offset,SEEK_SET);
		fseek(second_image,offset,SEEK_SET);
		bool already_put = false;
		printf("%d\n",max_text);
		for(int i = offset; i < max_first ; i++){
			buff_image = fgetc(first_image);
			if(get_bit(buff_image,1) == 0 && already_put==false){
				fputc(max_text,second_image);  /*metto la lunghezza del testo nel primo bit meno significativo , così da aiutarmi nella decodifica*/
				already_put=true;
				printf("%d\n",i);
			}else{
				fputc(buff_image,second_image);
			}
		}
		fseek(second_image,points[0]+offset,SEEK_SET);
		buff_image = fgetc(second_image);
		for(int i = 0 ;i < max_text;){ /*Copio il testo nei LSB dell'immagine*/
			if(get_bit(buff_image,1) == 0){
				fputc(buff_text,second_image);
				fseek(second_image,points[++i]+offset,SEEK_SET);
				buff_image = fgetc(second_image);
				buff_text = fgetc(text);
			}else{
				i--;
				buff_image = fgetc(second_image);
			}

		}
		free(points);
	}




	else if (mode == 0){
		printf("Decode\n");
		text=fopen(argv[4],"w");
		if (text== NULL) {
			fprintf(stderr, "Can't open text input file %s\n",argv[4]);
			exit(1);
		}
		int* points = get_points_esp();
		int img_length = get_message_length(second_image);
		int text_length = 0;
		char tmp_char_second;
		bool already_take = false;
		fseek(second_image,offset,SEEK_SET);
		for(int i = offset; i < img_length && already_take == false; i++){
			tmp_char_second = fgetc(second_image);
			if(get_bit(tmp_char_second, 1) == 0){
				text_length = (int)tmp_char_second;
				already_take = true;
				printf("%d\n",i);
				fflush(stdout);
			}
		}
		printf("%d\n", text_length);
	
		/* Grab LSB of all bytes for length specified at fgetc */
		
		int offset = get_image_data_offset(second_image);
		fseek(second_image,points[0]+offset,SEEK_SET);
		for(int i = 0 ; i < sizeof(points) ; i++){
			if(get_bit(tmp_char_second,1) == 0){
				fputc(tmp_char_second,text);
				tmp_char_second = fgetc(second_image);
				fseek(second_image,points[i]+offset,SEEK_SET);
			}else{
				i--;
			}
		}
		fclose(text);	
	}

	/* Clean up before exit */
	fclose(first_image);
	fclose(second_image);
}



int* get_points_esp(){ /*restituisce la porenza di 2 , è una semplice funzione che ti da la potenza di 2*/
	int* tmp = (int*)malloc(sizeof(int)*MAX_INT_POINTS);
	for(int i = 0; i < MAX_INT_POINTS ; i++){
		int c = i;
		int res = 1;
		while(c != 0){
			res *= 2;
			c--;
		}
		tmp[i]=res;
	}
	return tmp;
}

/*int* get_points_log(){
	int* tmp = (int*)malloc(sizeof(int)*MAX_INT_POINTS);
	for
}*/