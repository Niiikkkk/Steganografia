#include "Steganografia.hpp"

class context{
private:
	Steganografia* s = NULL;
public:
	void setState(Steganografia*);
	void exec();
};


void context::setState(Steganografia* s){
 	this -> s = s;
}

void context::exec(){
	s -> run();
}
