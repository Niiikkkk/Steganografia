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
	}else{
		printf("Decode...\n");
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