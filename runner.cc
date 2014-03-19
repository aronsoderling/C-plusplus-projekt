#include "mem_server.h"

#include <iostream>
#include <string>
#include <stdexcept>
#include <memory>
#include <string.h>

using namespace std;

int main(int argc, char* argv[]){
	if (argc != 3) {
		cerr << "Usage: myserver type port-number" << endl;
		exit(1);
	}
	
	int port = -1;
	try {
		port = stoi(argv[2]);
	} catch (exception& e) {
		cerr << "Wrong port number. " << e.what() << endl;
		exit(1);
	}
	
	unique_ptr<NewsServer> s;
	if(strcmp(argv[1], "memory") == 0){
		s = unique_ptr<NewsServer>( new MemServer(port) );
	}else if(strcmp(argv[1], "database") == 0){
		//s = unique_ptr<NewsServer>( new DBServer(port) );
	}else{
		cerr << "Wrong type. Type must be memory or database. " << endl;
		exit(1);
	}
	s->run();
}
