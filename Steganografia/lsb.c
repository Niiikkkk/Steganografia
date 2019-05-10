#include "class.hpp"
#define MAX_INT_POINTS 250

int* get_points_esp();

int* get_points_log();

int main(int argc , char** argv){
	if(argc < 2 || argc > 5){
		printf("***Steganografia***\nUSAGE : %s [-e]/[-d] and see istructions\n",*argv);
		return -1;
	}

	int modality = 2;

	if(!strcmp(argv[1],"-e"))
		modality = 1;
	else if(!strcmp(argv[1],"-d"))
		modality = 0;

	if(modality == 2){
		printf("***Steganografia***\nUSAGE : %s [-e]/[-d] and see istructions\n",*argv);
		return -1;
	}


	FILE* first_image;
	FILE* second_image;
	FILE* text;

	if(modality){  /*Modalita di Encode , metto nell'immagine il messaggio*/
		printf("Encode...\n");
		if(argc != 5){
			printf("***Steganografia***\nUSAGE : %s [-e]/[-d] SourceImage DestinationImage Text\n",*argv);
			return -1;
		}
		first_image = fopen(argv[2],"rb");
		if(first_image == NULL){
			printf("Error: Can't open the SourceImage\n");
			return -2;
		}
		second_image = fopen(argv[3],"wb");
		if(second_image == NULL){
			printf("Error: Can't create/write the DestinationImage\n");
			return -3;
		}
		text = fopen(argv[4],"r");
		if(text == NULL){
			printf("Error: Can't open the text\n");
			return -4;
		}


		/*code*/


		fclose(first_image);
		fclose(second_image);
		fclose(text);
	}else{
		printf("Decode...\n");
		if(argc != 4){
			printf("***Steganografia***\nUSAGE : %s [-e]/[-d] SourceImage TextDestination\n",*argv);
			return -1;
		}
		first_image = fopen(argv[2],"rb");
		if(first_image == NULL){
			printf("Error: Can't open the SourceImage\n");
			return -2;
		}
		text = fopen(argv[4],"r");
		if(text == NULL){
			printf("Error: Can't open the text\n");
			return -4;
		}


		/*code*/

		fclose(first_image);
	 	fclose(text);
	}
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