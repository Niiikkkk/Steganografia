#include "context.hpp"
	

#define MAX_INT_POINTS 250
#define ENCRIPT "-e"
#define DECRIPT "-d"

int main(int argc , char** argv){
	if(argc < 2 || argc > 5 || ((strcmp(argv[1],ENCRIPT)) && (strcmp(argv[1],DECRIPT)))){ /*vari controlli per riga di comando*/
		printf("***Steganografia***\nUSAGE : %s [-e]/[-d] and see istructions\n",*argv);
		return -1;
	}

	context* s = new context();

	if(!strcmp(argv[1],ENCRIPT)){		
		if(argc != 5){
			printf("***Steganografia***\nUSAGE : %s [-e] SourceImage DestinationImage Test\n",*argv);
			return -2;
		}
		Encript* e = new Encript();
		e -> setText(argv[4]);
		e -> setOrignialImage(argv[2]);
		e -> setDestImage(argv[3]);
		s -> setState(e);
	}
	if(!strcmp(argv[1],DECRIPT)){
		if(argc != 4){
			printf("***Steganografia***\nUSAGE : %s [-d] SourceImage Test\n",*argv);
			return -2;
		}
		Decript* d = new Decript();
		d -> setText(argv[3]);
		d -> setOrignialImage(argv[2]);
		s -> setState(d);
	}

	s -> exec();


	free(s);
}

