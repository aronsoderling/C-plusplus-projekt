#include "news_server.h"
#include "message_handler.h"
#include "server.h"
#include "connectionclosedexception.h"
#include "protocol.h"
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
				cout << "Received bytes" << endl;
				Command result = executeCommand(h.readMessage(*conn));
				cout << "Will now write response..." << endl;
				h.writeMessage(*conn, result);
				cout << "Response written" << endl;
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
	cout << "Command recieved: " << static_cast<Protocol::a> (c.id);
	for(Argument a : c.args){
		//cout << ch << " ";
	}
	cout << endl;
	return Command(Protocol::ANS_LIST_NG, vector<Argument>());
}
