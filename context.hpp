#include "Steganografia.hpp"

class context{
private:
	Steganografia* s = NULL;
public:
	void setState(Steganografia*);
	void exec();
	void setText(char*);
	void setOrignialImage(char*);
	void setDestImage(char*);
};


void context::setState(Steganografia* s){
 	this -> s = s;
}

void context::exec(){
	s -> run();
}

void context::setText(char* text){}

void context::setOrignialImage(char* fileName){}

void context::setDestImage(char* fileName){}