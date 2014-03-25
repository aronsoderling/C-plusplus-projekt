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
				cout << "Response: " << result << endl;
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
		if (createGroup(c.args[0].str_val)){
			args.push_back(Argument( Protocol::ANS_ACK ));
		} else{
			args.push_back(Argument(Protocol::ANS_NAK));
			args.push_back(Argument(Protocol::ERR_NG_ALREADY_EXISTS));
		}
		return Command(Protocol::ANS_CREATE_NG, args);

	} else if (c.id == Protocol::COM_DELETE_NG){
		if (deleteGroup(c.args[0].int_val)){
			args.push_back(Argument(Protocol::ANS_ACK));
		} else{
			args.push_back(Argument(Protocol::ANS_NAK));
			args.push_back(Argument(Protocol::ERR_NG_DOES_NOT_EXIST));
		}
		return Command(Protocol::ANS_DELETE_NG, args);

	} else if (c.id == Protocol::COM_LIST_ART){
		if (existsGroup(c.args[0].int_val)){
			vector<Article> articles = listArticles(c.args[0].int_val);
			args.push_back(Argument(Protocol::ANS_ACK));
			args.push_back(Argument(articles.size()));
			for (Article a : articles){
				args.push_back(Argument(a.getId()));
				args.push_back(Argument(a.getTitle()));
			}
		} else{
			args.push_back(Argument(Protocol::ANS_NAK));
			args.push_back(Argument(Protocol::ERR_NG_DOES_NOT_EXIST));
		}
		return Command(Protocol::ANS_LIST_ART, args);

	} else if (c.id == Protocol::COM_CREATE_ART){ //COM_CREATE_ART num_p string_p string_p string_p COM_END
		try{
			createArticle(c.args[0].int_val, c.args[1].str_val, c.args[2].str_val, c.args[3].str_val);
			args.push_back(Argument(Protocol::ANS_ACK));
		} catch(GroupDoesNotExistException e){
			args.push_back(Argument(Protocol::ANS_NAK));
			args.push_back(Argument(Protocol::ERR_NG_DOES_NOT_EXIST));
		}
		return Command(Protocol::ANS_CREATE_ART, args);

	} else if (c.id == Protocol::COM_DELETE_ART){	//COM_DELETE_ART num_p num_p COM_END
							//ANS_DELETE_ART[ANS_ACK | ANS_NAK[ERR_NG_DOES_NOT_EXIST | ERR_ART_DOES_NOT_EXIST]] ANS_END
		try{
			deleteArticle(c.args[0].int_val, c.args[1].int_val);
			args.push_back(Argument(Protocol::ANS_ACK));
		} catch (ArticleDoesNotExistException& e){
			args.push_back(Argument(Protocol::ANS_NAK));
			args.push_back(Argument(Protocol::ERR_ART_DOES_NOT_EXIST));
		} catch (GroupDoesNotExistException& e){
			args.push_back(Argument(Protocol::ANS_NAK));
			args.push_back(Argument(Protocol::ERR_NG_DOES_NOT_EXIST));
		}
		return Command(Protocol::ANS_DELETE_ART, args);

	} else if (c.id == Protocol::COM_GET_ART){	//COM_GET_ART num_p num_p COM_END
			//ANS_GET_ART[ANS_ACK string_p string_p string_p | ANS_NAK[ERR_NG_DOES_NOT_EXIST | ERR_ART_DOES_NOT_EXIST]] ANS_END
		try{
			Article a = getArticle(c.args[0].int_val, c.args[1].int_val);
			args.push_back(Argument(Protocol::ANS_ACK));
			args.push_back(a.getTitle());
			args.push_back(a.getAuthor());
			args.push_back(a.getText());
		} catch (ArticleDoesNotExistException& e){
			args.push_back(Argument(Protocol::ANS_NAK));
			args.push_back(Argument(Protocol::ERR_ART_DOES_NOT_EXIST));
		} catch (GroupDoesNotExistException& e){
			args.push_back(Argument(Protocol::ANS_NAK));
			args.push_back(Argument(Protocol::ERR_ART_DOES_NOT_EXIST));
		}
		return Command(Protocol::ANS_GET_ART, args);
	} else {
		cerr << "OOPS! Something is broken (command id: " << c.id << " mapped to:" << Protocol::map(c.id) << ")" << endl;
	}
	return Command(Protocol::ANS_NAK, vector<Argument>());
}
