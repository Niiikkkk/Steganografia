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
		s -> setState(new Encript());
		s-> setDestImage(argv[3]);
	}
	if(!strcmp(argv[1],DECRIPT)){
		if(argc != 4){
			printf("***Steganografia***\nUSAGE : %s [-d] SourceImage Test\n",*argv);
			return -2;
		}
		s -> setState(new Decript());
	}

	s-> setText(argv[4]);
	s-> setOriginalImage(argv[2]);



	s -> exec();


	free(s);
}

