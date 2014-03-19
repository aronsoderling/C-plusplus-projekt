#include "news_server.h"
#include "message_handler.h"
#include "server.h"
#include "connectionclosedexception.h"
#include <iostream>
#include <string>

using namespace std;

void NewsServer::run(){
	MessageHandler h;
	
	Server server(port);

	while (true) {
		auto conn = server.waitForActivity();
		if (conn != nullptr) {
			try {
				Command result = executeCommand(h.readMessage(*conn));
				h.writeMessage(*conn, result);
			} catch (ConnectionClosedException&) {
				server.deregisterConnection(conn);
				cout << "Client closed connection" << endl;
			}
		} else {
			conn = make_shared<Connection>();
			server.registerConnection(conn);
			cout << "New client connects" << endl;
		}
	}
}

Command NewsServer::executeCommand(Command c){
	cout << "Command recieved: " << c.name << " ";
	for(string s : c.args){
		cout << s << " ";
	}
	cout << endl;

	return Command("list", vector<string>());
}
