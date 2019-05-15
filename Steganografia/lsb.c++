#include "ByteHandler.hpp"
#define MAX_INT_POINTS 250

int* getPoints(int,int);

int main(int argc , char** argv){
	if(argc < 2 || argc > 5){ /*vari controlli per riga di comando*/
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

	FILE* first_image; /*Dichiarazione dei file da utilizzare sia nella fase di encode che nella decode*/
	FILE* text;

	ByteHandler* byte = new ByteHandler(); /*dichiarazione della magnilia per la gestione dei byte*/
	if(modality){  /*Modalita di Encode , metto nell'immagine il messaggio*/
		FILE* second_image;
		printf("Encode...\n");
		if(argc != 5){  /*Controlli...*/
			printf("***Steganografia***\nUSAGE : %s [-e]/[-d] SourceImage DestinationImage Text\n",*argv);
			return -1;
		}
		first_image = fopen(argv[2],"rb"); /*apertura dei file*/
		if(first_image == NULL){
			printf("Error: Can't open the SourceImage\n");
			return -2;
		}
		second_image = fopen(argv[3],"wb+"); /*sia read and write*/
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



		for(int i = 0 ; i< dataOffset ; i++){  /*Copia l'header nella seconda immagine*/
			bufferFirst = fgetc(first_image);
			fputc(bufferFirst,second_image);
		}
		
		

		
		textLength = byte->getLength(text);   /*prende la lunghezza del testo*/
		printf("Lunghezza del testo: %d\n",textLength);

		srand(time(NULL));
		int num = rand() % 100;

		int* points = byte -> getPoints(textLength,firstLength,num);  /*si prende tutti i punti, su cui poi andare a mettere i byte modificati*/







		char s[100];
		sprintf(s,"%d %d",textLength,num);

		fseek(second_image,dataOffset,0); /*la fseek sposta il puntatore sul secondo paramentro , partendo dal terzo*/
		fputc(s,second_image); /*mette in una posizione fissa , dove iniziano i dati, la lunghezza del testo , che è in points[0]*/












		int count = 1; /*count parte da 1 perchè in 0 c'è la lunghezza del testo*/
		fseek(text,0,SEEK_SET); /*sposta il puntatore del testo all'inizio*/
		int loop = 0;
		bufferText = fgetc(text); /*prende il primo byte del testo*/
		char bufferSecondMod = 0b00000000;


		fseek(first_image,dataOffset,SEEK_SET); /*sposta il puntatore dell'immagine*/


		for(int i = dataOffset; i < firstLength; i++){

			char c = fgetc(first_image);

			if(i == points[count]+dataOffset){
				if(!feof(text)){  /*finchè non finisce il testo*/
					printf("Byte IMG BEFORE:");
					byte -> printByte(c); /*visualizza il byte dell'immagine prima di essere modificata*/

					printf(" Text:%d  ",byte-> getBit(bufferText,8-loop)); /*visualizza il loopesimo byte del tetso*/
					printf("Last bit img before modify:%d   ",byte-> getBit(c,1)); /*VIsualizza il 1 byte della immagine prima di essere modificata*/


					bufferSecondMod = byte -> compareBitsLSB(bufferText,c, 8-loop);  /*gurda se i bit sono uguali , altrimenti li cambia*/
					printf("Last bit img after modify:%d    ",byte-> getBit(bufferSecondMod,1));  /*visualizza il 1 bit dell0immagine dopo la modifica*/
					fseek(second_image,i,SEEK_SET);
					fputc(bufferSecondMod,second_image); /*mette il byte modificato nell'immagine*/

					printf("Byte IMG AFTER:"); /*visualizza il byte dell immagine dopo la modifica*/
					byte -> printByte(bufferSecondMod);				

					if(loop == 7){ /*se loop == 7 , ovvero se ha visto tutto il byte*/
						printf("Byte finito\n\n\n");
						loop = 0;
						bufferText = fgetc(text);						
					}else{
						loop++;
					}
					count++;
					
				}else{
					count = 0;
				}
			}else{
				fputc(c,second_image);
			}
		}
		

		fclose(first_image); /*chiude i file*/
		fclose(second_image);
		fclose(text);




	}else{



		printf("Decode...\n"); /*COntrolli..*/
		if(argc != 4){
			printf("***Steganografia***\nUSAGE : %s [-e]/[-d] SourceImage TextDestination\n",*argv);
			return -1;
		}
		first_image = fopen(argv[2],"rb");
		if(first_image == NULL){
			printf("Error: Can't open the SourceImage\n");
			return -2;
		}
		text = fopen(argv[3],"w");
		if(text == NULL){
			printf("Error: Can't open the text\n");
			return -4;
		}
		fseek(first_image,10,SEEK_SET); /*in 10 dell'header c'è l'offset dei dati*/
		int dataOffset = fgetc(first_image);
		printf("Dove iniziano i dati: %d\n",dataOffset);
		fseek(first_image , dataOffset, SEEK_SET);







		char* s = fgetc(first_image);
		printf("%s\n",s);

		int num = (int)fgetc(first_image);
		printf("Numero random: %d\n",num);
		printf("Lunghezza del tsto: %d\n",textLength);








		

		int imageLength = byte -> getLength(first_image);

		int* points = byte ->getPoints(textLength,imageLength,num); /*si prende i punti*/

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
				fseek(first_image,i,SEEK_SET);			
				buffRead = fgetc(first_image);

				printf("Byte:");
				byte -> printByte(buffRead);
				printf("\n");

				//printf("First image:%d\n",byte->getBit(buffRead,1));
				int bit = byte -> getBit(buffRead,1);
				result = byte -> changeBit(result,8-loop,bit);
				if(loop == 7){
					finalresult[offsetText] = result;
					offsetText ++;
					fputc(result,text);
					result = 0b00000000;
					loop = 0;
					printf("\n\n");
				}else
					loop++;
				count++;			
			}
		}
		printf("%d\n",count);
		printf("%s\n",finalresult);		

		fclose(first_image);
	 	fclose(text);
	}
}
