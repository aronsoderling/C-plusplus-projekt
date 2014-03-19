#include "connection.h"
#include "connectionclosedexception.h"
#include "message_handler.h"
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>

using namespace std;

string readString(const Connection& conn) {
	string s;
	char ch;
	while ((ch = conn.read()) != '$') {
		s += ch;
	}
	return s;
}

int main(int argc, char* argv[]) {
	if (argc != 3) {
		cerr << "Usage: myclient host-name port-number" << endl;
		exit(1);
	}
	
	int port = -1;
	try {
		port = stoi(argv[2]);
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
		try {
			cout << cmd_str << " > ";
			h.writeMessage(conn, c);
			string reply = readString(conn);
			cout << " " << reply << endl;
			cout << "Type another number: ";
		} catch (ConnectionClosedException&) {
			cout << " no reply from server. Exiting." << endl;
			exit(1);
		}
	}
}

