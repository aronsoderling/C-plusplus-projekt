#include "connection.h"
#include "connectionclosedexception.h"
#include "message_handler.h"
#include "command.h"
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include <stdlib.h> // strtol

using namespace std;

void printResponse(const Command& cmd){
	stringstream ss;
	if (cmd.ok()){
		switch (cmd.id){
		case Protocol::ANS_CREATE_ART: ss << "Article was created successfully \n"; break;
		case Protocol::ANS_CREATE_NG: ss << "News group was created successfully \n";break;
		case Protocol::ANS_DELETE_ART: ss << "Article was deleted successfully \n"; break;
		case Protocol::ANS_DELETE_NG: ss << "News group was deleted successfully \n"; break;
		case Protocol::ANS_GET_ART: 
			ss << cmd.args[1].str_val << '\t' << "Author: " << cmd.args[2].str_val << '\n' 
					<< cmd.args[3].str_val << '\n'; 
			break;
		case Protocol::ANS_LIST_ART:
			for (size_t i = 2; i < cmd.args.size(); i += 2){
				ss << cmd.args[i].int_val << ". ";
				ss << cmd.args[i + 1].str_val << '\n';
			}
			break;
		case Protocol::ANS_LIST_NG: 
			for (size_t i = 1; i < cmd.args.size(); i += 2){
				ss << cmd.args[i].int_val << ". ";
				ss << cmd.args[i + 1].str_val << '\n';
			}
			break;
		default: break;
		}
	} else {
		switch (cmd.id){
		case Protocol::ANS_CREATE_ART: ss << "Unable to create article. "; break;
		case Protocol::ANS_CREATE_NG: ss << "Unable to create news group. "; break;
		case Protocol::ANS_DELETE_ART: ss << "Unable to delete article. "; break;
		case Protocol::ANS_DELETE_NG: ss << "Unable to delete article. "; break;
		case Protocol::ANS_GET_ART: ss << "Unable to create article. "; break;
		case Protocol::ANS_LIST_ART: ss << "Unable to list articles. "; break;
		default: break;
		}
		ss << cmd.errMsg() << '\n';
	}
	cout << ss.str();
}

void showHelp(){
	cout << "Available commands: " << endl;
	cout << "   listg				- list groups " << endl;
	cout << "   createg name				- create group " << endl;
	cout << "   deleteg group-id			- delete group " << endl;
	cout << "   lista group-id			- list articles in group" << endl;
	cout << "   createa group-id title author text	- create article " << endl;
	cout << "   deletea group-id article-id		- delete article" << endl;
	cout << "   read group-id article-id		- read article" << endl;
	cout << "   help				- show this help " << endl;
	cout << "   quit				- exit the program " << endl;
	cout << "news> ";
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

	showHelp();

	string cmd_str;
	while (getline(cin, cmd_str)) {
		try{
			Command c(cmd_str);
			//clog << "String '" << cmd_str << "' converted into command " << c << endl;
			h.writeMessage(conn, c);
			Command response = h.readMessage(conn);
			printResponse(response);
			cout << "news> ";
		} catch (InvalidCommandException& e){
			if (cmd_str == "help"){
				showHelp();
				continue;
			} else if (cmd_str == "exit"){
				exit(0);
			}
			cout << "Invalid command. " << endl;
			cout << "news> ";
		} catch (ConnectionClosedException& e) {
			cout << "No reply from server. Exiting." << endl;
			exit(1);
		}
	}
}