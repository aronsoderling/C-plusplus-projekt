#include "connection.h"
#include "connectionclosedexception.h"
#include "message_handler.h"
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include <stdlib.h> // strtol

using namespace std;

string readString(const Connection& conn) {
	string s;
	char ch;
	cout << "here11" << endl;
	while (ch = conn.read()) {
		s += ch;
		cout << ch << endl;
	}
	cout << "here12" << endl;
	return s;
}

void printResponse(Command res){

}

int main(int argc, char* argv[]) {
	if (argc != 3) {
		cerr << "Usage: myclient host-name port-number" << endl;
		exit(1);
	}
	
	int port = -1;
	try {
		port = strtol(argv[2], 0, 10); // stoi(argv[2]); does not work in cygwin
	} catch (exception& e) {
		cerr << "Wrong port number. " << e.what() << endl;
		exit(1);
	}
	
	Connection conn(argv[1], port);
	if (!conn.isConnected()) {
		cerr << "Connection attempt failed" << endl;
		exit(1);

	}

	MessageHandler h;	


	cout << "Enter command: ";
	string cmd_str;
	while (getline(cin, cmd_str)) {
		Command c(cmd_str);
		cout << "String '" << cmd_str << "' converted into command " << c << endl;

		try {
			h.writeMessage(conn, c);
			Command response = h.readMessage(conn);
			printResponse(response);
			cout << "Type another number: ";
		} catch (ConnectionClosedException&) {
			cout << " no reply from server. Exiting." << endl;
			exit(1);
		}
	}
}

