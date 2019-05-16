#include "ByteHandler.hpp"

class Steganografia{
	public:
		virtual void run() = 0;
		virtual void setText()  = 0;
		virtual void setOrignialImage() = 0;
		virtual void setDestImage() = 0;
};


class Encript: public Steganografia{
	public:
		int encript(int,char**);
		void run();
		void setText();
		void setOrignialImage();
		void setDestImage();
	private:
		ByteHandler* byte = NULL;
};


void Encript::run(){
	printf("Encript\n");
}

int Encript::encript(int argc,char** argv){
	byte = new ByteHandler();
	FILE* first_image; /*Dichiarazione dei file da utilizzare sia nella fase di encode che nella decode*/
	FILE* text;
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

		int count = 1; /*count parte da 1 perchè in 0 c'è la lunghezza del testo*/
		fseek(text,0,SEEK_SET); /*sposta il puntatore del testo all'inizio*/
		int loop = 0;
		bufferText = fgetc(text); /*prende il primo byte del testo*/
		char bufferSecondMod = 0b00000000;


		fseek(first_image,dataOffset,SEEK_SET); /*sposta il puntatore dell'immagine*/


		for(int i = dataOffset; i < firstLength; i++){

			if(i == dataOffset){  /*mette in una posizione fissa , dove iniziano i dati, la lunghezza del testo , che è in points[0]*/
				for(int j = 0 ; j < 8 ; j++){
					fseek(first_image,dataOffset+j,SEEK_SET);
					char c = fgetc(first_image);
					bufferSecondMod = byte -> compareBitsLSB(textLength,c, 8-j);
					fseek(second_image,dataOffset+j,SEEK_SET);
					fputc(bufferSecondMod,second_image);
				}
				printf("text:%d\n",textLength);
			}
			if(i == dataOffset + 8){
				for(int j = 8 ; j < 16 ; j++){
					fseek(first_image,dataOffset+j,SEEK_SET);
					char c = fgetc(first_image);
					bufferSecondMod = byte -> compareBitsLSB(num,c, 16-j);
					fseek(second_image,dataOffset+j,SEEK_SET);
					fputc(bufferSecondMod,second_image);
				}
				printf("num:%d\n",num);
			}

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
					printf("\n");

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

		return 0;
}

class Decript: public Steganografia{
	public:
		int decript(int,char**);
		void run();
		void setText();
		void setOrignialImage();
		void setDestImage();
	private:
		ByteHandler* byte = NULL;
};

void Decript::run(){
	printf("Decript\n");
}


int Decript::decript(int argc, char** argv){
	printf("Decode...\n"); /*COntrolli..*/

	FILE* first_image;
	FILE* text;

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

	int textLength = 0;
	int num = 0;


	int imageLength = byte -> getLength(first_image);

	int count = 1;
	int offsetText = 0;
	int loop = 0;
	char buffRead;
	char result = 0b00000000;
	int* points = (int*)malloc(sizeof(int)*imageLength/8);
	char* finalresult = (char*)malloc(sizeof(char)*imageLength/8);


	for(int i = dataOffset; i < imageLength ; i++){



		if(i == dataOffset){  //va a dataOffset e prende gli ultimi bit per otto byte e li mette in textLength
			for(int j = 0 ; j < 8 ; j++){
				fseek(first_image,dataOffset+j,SEEK_SET);			
				buffRead = fgetc(first_image);
				int bit = byte -> getBit(buffRead,1);
				result = byte -> changeBit(result,8-j,bit);				
			}
			textLength = (int) result;
		}
		if(i == dataOffset + 8){  //va a dataOffset+8,gli altri sono per il textLength, e prende gli ultimi bit per otto byte e li mette in num
			for(int j = 8 ; j < 16 ; j++){
				fseek(first_image,dataOffset+j,SEEK_SET);			
				buffRead = fgetc(first_image);
				int bit = byte -> getBit(buffRead,1);
				result = byte -> changeBit(result,16-j,bit);		
			}
			num = (int) result;
		}
		if(i == dataOffset + 16){  //se ha finito di prendere il num , finisce al 15 byte , allora prendeti i punti 
			points = byte ->getPoints(textLength,imageLength,num); /*si prende i punti*/
			
		}


		if(i == points[count]+dataOffset){
			fseek(first_image,i,SEEK_SET);			
			buffRead = fgetc(first_image);

			printf("Byte:");
			byte -> printByte(buffRead);
			printf("\n");

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