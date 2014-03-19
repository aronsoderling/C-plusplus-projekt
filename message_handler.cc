#include "message_handler.h"
#include "connection.h"
#include <vector>
#include <string>
#include <sstream>

using namespace std;

MessageHandler::MessageHandler() {
}


Command MessageHandler::readMessage(const Connection& conn) const{
	stringstream ss;
	string name;
	vector<string> args;
	string arg;
	
	char byte;

	unsigned char byte1 = conn->read();
	unsigned char byte2 = conn->read();
	unsigned char byte3 = conn->read();
	unsigned char byte4 = conn->read();
	int id = (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;

	switch(id){
	case COM_LIST_NG: name = "list"; break;
	case COM_LIST_ARG: name = "list"; break;
	case COM_CREATE_NG: name = "create"; break;
	case COM_DELETE_NG: name = "create"; break;
	case COM_LIST_ART: name = "create"; break;
	case COM_CREATE_ART: name = "create"; break;
	case COM_DELETE_ART: name = "create"; break;
	case COM_GET_ART: name = "create"; break;
	case COM_END: name = "create"; break;

	case ANS_LIST_NG: name = "create"; break;
	case ANS_CREATE_NG: name = "create"; break;
	case ANS_DELETE_NG: name = "create"; break;
	case ANS_LIST_ART: name = "create"; break;
	case ANS_CREATE_ART: name = "create"; break;
	case ANS_DELETE_ART: name = "create"; break;
	case ANS_GET_ART: name = "create"; break;
	case ANS_END: name = "create"; break;
	case ANS_ACK: name = "create"; break;
	case ANS_NAK: name = "create"; break;

	case PAR_STRING: name = "create"; break;
	case PAR_NUM: name = "create"; break;

	case ERR_NG_ALREADY_EXISTS: name = "create"; break;
	case ERR_NG_DOES_NOT_EXIST: name = "create"; break;
	case ERR_ART_DOES_NOT_EXIST: name = "create"; break;
	case ERR_UNSPECIFIED: name = "create"; break;
	}
	
	unsigned char byte = conn.read();
	while(byte != COM_END && byte != ANS_END){
		ss << byte;
	}
	
	getline(ss, name, ' ');
	while(getline(ss, arg, ' ')){
		args.push_back(arg);
	}

	return Command(name, args);
}

void MessageHandler::writeMessage(const Connection& conn, const Command cmd) const{
	int id = 0;
	if(cmd.name == "list"){
		if(cmd.args.size() == 0){
			id = COM_LIST_NG;
		}else if(cmd.args.size() == 1){
			id = COM_LIST_ART;
		}
	}

	conn.write((id >> 24) & 0xFF);
	conn.write((id >> 16) & 0xFF);
	conn.write((id >> 8)  & 0xFF);
	conn.write(id & 0xFF);

	conn.write('$');
}
