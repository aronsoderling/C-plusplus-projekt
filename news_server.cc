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
				//cout << "Received bytes" << endl;
				Command result = executeCommand(h.readMessage(*conn));
				//cout << "Will now write response..." << endl;
				h.writeMessage(*conn, result);
				//cout << "Response written" << endl;
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
	cout << "Command recieved: " << c << endl;
	vector<Argument> args;

	if (c.id == Protocol::COM_LIST_NG){
		vector<Group> groups = listGroups();
		args.push_back( Argument(groups.size()) );
		for (Group g : groups){
			args.push_back(Argument(g.getId()));
			args.push_back(Argument(g.getName()));
		}
		return Command(Protocol::ANS_LIST_NG, args);
	} else if (c.id == Protocol::COM_CREATE_NG){
		createGroup(c.args[0].str_val);
	}
	return Command(Protocol::ANS_NAK, vector<Argument>());
}
