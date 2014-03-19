#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include "connection.h"
#include <string>
#include <sstream>
#include <vector>

const int COM_LIST_NG = 1,
	COM_CREATE_NG = 2,
	COM_DELETE_NG = 3,
	COM_LIST_ART = 4,
	COM_CREATE_ART = 5,
	COM_DELETE_ART = 6,
	COM_GET_ART = 7,
	COM_END = 8,

	ANS_LIST_NG = 20,
	ANS_CREATE_NG = 21,
	ANS_DELETE_NG = 22,
	ANS_LIST_ART = 23,
	ANS_CREATE_ART = 24,
	ANS_DELETE_ART = 25,
	ANS_GET_ART = 26,
	ANS_END = 27,
	ANS_ACK = 28,
	ANS_NAK = 29,

	PAR_STRING = 40,
	PAR_NUM = 41,

	ERR_NG_ALREADY_EXISTS = 50,
	ERR_NG_DOES_NOT_EXIST = 51,
	ERR_ART_DOES_NOT_EXIST = 52,
	ERR_UNSPECIFIED = 53;

struct Command{
	Command(std::string str){	
		std::string res;
		std::stringstream ss;
		ss << str;
		std::string cmd_name;
		ss >> cmd_name;
	
		while(getline(ss, res, ' ')){
			args.push_back(res);
		}

		Command(cmd_name, args);
	}
	
	Command(std::string n, std::vector<std::string> a) : name(n), args(a) {};
	std::string name;
	std::vector<std::string> args; 
};

class MessageHandler {
public:
	MessageHandler();
	
	void writeMessage(const Connection& conn, const Command cmd) const;
	Command readMessage(const Connection& conn) const;
	
private:

};

#endif
