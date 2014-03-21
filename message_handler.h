#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include "connection.h"
#include "protocol.h"
#include <string>
#include <sstream>
#include <vector>
#include <iostream>

struct Command{
	Command(std::string str){	
		char res;
		std::stringstream ss;
		ss << str;
		std::string cmd_name;
		ss >> cmd_name;
	
		while(ss >> res){
			args.push_back(res);
		}
		
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

		Command(cmd_id, args);
	}

	Command(std::vector<char> bytes){
		id = bytes[0];
		for (int i = 1; i-1 < bytes.size(); ++i){
			args.push_back(bytes[i]);
		}
	}
	
	Command(char i, std::vector<char> a) : id(i), args(a) {};
	char id;
	std::vector<char> args; 
};

class MessageHandler {
public:
	MessageHandler();
	
	void writeMessage(const Connection& conn, const Command cmd) const;
	Command readMessage(const Connection& conn) const;
	static void writeInt(const Connection& conn, int value);
	static int readInt(const Connection& conn);
private:

};

#endif
