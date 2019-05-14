#include "Byte_Handler.hpp"
#define MAX_INT_POINTS 250

int* getPoints(int);

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

	ByteHandler* byte = new ByteHandler();
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
		second_image = fopen(argv[3],"w+"); /*sia read and write*/
		if(second_image == NULL){
			printf("Error: Can't create/write the DestinationImage\n");
			return -3;
		}
		text = fopen(argv[4],"rb");
		if(text == NULL){
			printf("Error: Can't open the text\n");
			return -4;
		}


		
		int dataOffset = byte-> getImageDataOffset(first_image);
		char bufferFirst;
		char bufferSecond;
		char bufferText;
		int textLength = 0;
		int firstLength = byte -> getLength(first_image);
		printf("Lunghezza della prima immagine: %d\n",firstLength);
		for(int i = 0 ; i< dataOffset ; i++){  /*Copia l'header e i dati nella seconda immagine*/
			bufferFirst = fgetc(first_image);
			fputc(bufferFirst,second_image);
		}
		
		

		int imageLength = byte -> getLength(second_image);
		printf("Lunghezza della seconda immagine: %d\n",imageLength);

		
		textLength = byte->getLength(text);
		printf("Lunghezza del testo: %d\n",textLength);


		int* points = getPoints(textLength);

		fseek(second_image,dataOffset,0);
		fputc(points[0],second_image);

		fseek(second_image,dataOffset,0);
		printf("%d\n",fgetc(second_image));


		printf("\n%d",dataOffset);

		printf("Lunghezza massima : %d\n",points[0]);

		int count = 1;
		fseek(text,0,SEEK_SET);
		int loop = 0;
		bufferText = fgetc(text);
		for(int i = dataOffset+1; i < firstLength; i++){
			fseek(first_image,i,SEEK_SET);
			char c = fgetc(first_image);
			if(i == points[count]+dataOffset){
				if(!feof(text)){
					
					for(int i = 0; i <8 ; i++){
						printf("%d", byte->getBit(c,8-i));
					}

					printf(" Text:%d  ",byte-> getBit(bufferText,8-loop));
					printf("Last bit img before modify:%d   ",byte-> getBit(c,1));


					char bufferSecondMod = byte -> compareBitsLSB(bufferText,c, 8-loop);
					printf("Last bit img after modify:%d    ",byte-> getBit(bufferSecondMod,1));
					fseek(second_image,i,dataOffset);
					fputc(bufferSecondMod,second_image);

					for(int i = 0; i <8 ; i++){
						printf("%d", byte->getBit(bufferSecondMod,8-i));
					}

					fseek(second_image,i,dataOffset);

					printf(" Image:");
					for(int i = 0; i <8 ; i++){
						printf("%d", byte->getBit(fgetc(second_image),8-i));
					}
					printf("\n");

					if(loop == 7){
						printf("Byte finito\n\n\n");
						loop = 0;
						bufferText = fgetc(text);						
					}else
						loop++;
					count++;
					
				}else{
					count = 0;
				}
			}else{
				fputc(c,second_image);
			}
		}
		
		/*while(!feof(text)){
			bufferText = fgetc(text);
			for(int j = 0; j<=7 ; j++){
				fseek(first_image,points[count],dataOffset);
				bufferSecond = fgetc(first_image);

				printf("Text:%d\n",byte-> getBit(bufferText,8-j));
				printf("Image first modify:%d\n",byte-> getBit(bufferSecond,1));


				char bufferSecondMod = byte -> compareBitsLSB(bufferText,bufferSecond , 8-j);
				printf("Image after modify:%d\n",byte-> getBit(bufferSecondMod,1));
				fseek(second_image,points[count],dataOffset);
				fputc(bufferSecondMod,second_image);


				fseek(second_image,points[count],dataOffset);
				bufferSecond = fgetc(second_image);


				printf("Image:%d\n\n",byte-> getBit(bufferSecond,1));
				count++;
			}
		}*/

		fseek(second_image,points[1],dataOffset);
		char c = fgetc(second_image);
		printf("%d\n",byte-> getBit(c,1));



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
		printf("Dove iniziano i dati: %d\n",dataOffset);

		fseek(first_image , dataOffset, SEEK_SET);
		int textLength = (int)fgetc(first_image);
		printf("Lunghezza del tsto: %d\n",textLength);

		int imageLength = byte -> getLength(first_image);




		int* points = getPoints(textLength);

		int count = 1;
		int offsetText = 0;
		int loop = 0;
		char buffRead;
		char result = 0b00000000;
		char* finalresult = (char*)malloc(sizeof(char)*textLength);

		printf("OK\n");
		fflush(stdout);

		for(int i = dataOffset; i < imageLength ; i++){
			if(i == points[count]+dataOffset){
				fseek(first_image,points[count],dataOffset);
				count++;
				buffRead = fgetc(first_image);
				printf("First image:%d\n",byte->getBit(buffRead,1));
				int bit = byte -> getBit(buffRead,1);
				result = byte -> changeBit(result,8-loop,bit);
				loop++;
				if(loop == 7){
					finalresult[offsetText] = result;
					offsetText ++;
					fputc(result,text);
					result = 0b00000000;
					loop = 0;
				}
			}
		}
		printf("%s\n",finalresult);		

		fclose(first_image);
	 	fclose(text);
	}
}



int* getPoints(int length){ /*restituisce la porenza di 2 , è una semplice funzione che ti da la potenza di 2*/
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
	int* points = (int*)malloc(sizeof(int)*length*8);
	points[0] = length;
	for(int i = 1 ; i< length*8; i++){
		points[i] = i;
	}
	return points;
}

/*int* get_points_log(){
	int* tmp = (int*)malloc(sizeof(int)*MAX_INT_POINTS);
	for
}*/