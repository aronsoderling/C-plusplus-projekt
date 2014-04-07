#include "mem_server.h"
#include "db_server.h"

#include <iostream>
#include <string>
#include <stdexcept>
#include <memory>
#include <string.h>
#include <stdlib.h> // strtol

using namespace std;

int main(int argc, char* argv[]){
	if (argc != 3) {
		cerr << "Usage: myserver type port-number" << endl;
		return 1;
	}
	
	int port = -1;
	try {
		port = strtol(argv[2], 0, 10); // stoi(argv[2]); does not work in cygwin
	} catch (exception& e) {
		cerr << "Wrong port number. " << e.what() << endl;
		return 1;
	}
	
	unique_ptr<NewsServer> s;
	if(strcmp(argv[1], "memory") == 0){
		s = unique_ptr<NewsServer>( new MemServer(port) );
	}else if(strcmp(argv[1], "database") == 0){
		s = unique_ptr<NewsServer>( new DbServer(port) );
	}else{
		cerr << "Wrong type. Type must be memory or database. " << endl;
		return 1;
	}
	s->run();
}
