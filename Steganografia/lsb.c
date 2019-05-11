#include "Byte_Handler.hpp"
#define MAX_INT_POINTS 250

int* getPoints(int, ByteHandler* );

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
		text = fopen(argv[4],"rb");
		if(text == NULL){
			printf("Error: Can't open the text\n");
			return -4;
		}


		ByteHandler* byte = new ByteHandler();
		int dataOffset = byte-> getImageDataOffset(first_image);
		char bufferFirst;
		char bufferSecond;
		char bufferText;
		int textLegth = 0;
		int firstLength = byte -> getLength(first_image);
		
		for(int i = 0 ; i< firstLength ; i++){  /*Copia l'header e i dati nella seconda immagine*/
			bufferFirst = fgetc(first_image);
			fputc(bufferFirst,second_image);
		}

		
		

		int imageLength = byte -> getLength(second_image);
		textLegth = byte->getLength(text);


		int* points = getPoints(textLegth,byte);


		fseek(second_image,dataOffset,SEEK_SET);
		fputc((char)points[0],second_image);


		printf("Lunghezza massima : %d\n",points[0]);

		fseek(second_image,points[1]+dataOffset,SEEK_SET);
		fseek(text,0,SEEK_SET);
		for(int i = dataOffset ; i < imageLength; i+=8){
			if(!feof(text)){
				bufferSecond = fgetc(second_image);
				bufferText = fgetc(text);
				for(int j = 0; j<8 ; j++){
					char bufferSecondMod = byte -> compareBitsLSB(bufferSecond , bufferText , j);
					fputc(bufferSecondMod,second_image);
					fseek(second_image,points[j+i]+dataOffset,SEEK_SET);
					bufferSecond = fgetc(second_image);
				}
			}else{
				i = imageLength;
			}
		}



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
		text = fopen(argv[3],"wb");
		if(text == NULL){
			printf("Error: Can't open the text\n");
			return -4;
		}
		fseek(first_image,10,SEEK_SET);
		int dataOffset = fgetc(first_image);
		printf("%d\n",dataOffset);

		fseek(first_image , dataOffset, SEEK_SET);
		int textLength = fgetc(first_image);
		printf("%d",textLength);


		

		fclose(first_image);
	 	fclose(text);
	}
}



int* getPoints(int length,ByteHandler* byte){ /*restituisce la porenza di 2 , è una semplice funzione che ti da la potenza di 2*/
	/*int* tmp = (int*)malloc(sizeof(int)*MAX_INT_POINTS);
	for(int i = 0; i < MAX_INT_POINTS ; i++){
		int c = i;
		int res = 1;
		while(c != 0){
			res *= 2;
			c--;
		}
		tmp[i]=res;
	}
	return tmp;*/
	int* points = (int*)malloc(sizeof(int)*length);
	points[0] = length;
	for(int i = 2 ; i< length; i++){
		points[i] = i;
	}
	return points;
}

/*int* get_points_log(){
	int* tmp = (int*)malloc(sizeof(int)*MAX_INT_POINTS);
	for
}*/