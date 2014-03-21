#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include "connection.h"
#include "protocol.h"
#include <string>
#include <sstream>
#include <vector>
#include <iostream>

struct Argument{
	Argument(std::string v) : str_val(v), type(Protocol::PAR_STRING){};
	Argument(int v) : int_val(v), type(Protocol::PAR_NUM){};
	char type;
	std::string str_val;
	int int_val;
};

struct Command{
	Command(std::string str){	
		std::string res;
		std::stringstream ss;
		ss << str;
		std::string cmd_name;
		ss >> cmd_name;
		
		char cmd_id;
		if (cmd_name == "list"){
			if (args.size() == 0){
				cmd_id = Protocol::COM_LIST_NG;
			}else if (args.size() == 1){
				cmd_id = Protocol::COM_LIST_ART;
			}
		}else if (cmd_name == "create"){

		}else{
			std::cout << "Unknown command" << std::endl;
		}

		while (ss >> res){
			if (cmd_name == "list"){
				args.push_back(Argument(res));
			} else if (cmd_name == "something else"){

			}
		}

		Command(cmd_id, args);
	}
	Command(char i, std::vector<Argument> a) : id(i), args(a) {};

	
	char id;
	std::vector<Argument> args; 
};

class MessageHandler {
public:
	MessageHandler();
	
	void writeMessage(const Connection& conn, const Command cmd) const;
	Command readMessage(const Connection& conn) const;
	static void writeString(const Connection& conn, std::string value);
	static std::string readString(const Connection& conn);
	static void writeInt(const Connection& conn, int value);
	static int readInt(const Connection& conn);
private:

};

#endif
