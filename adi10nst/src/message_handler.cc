#include "message_handler.h"
#include "connection.h"
#include "protocol.h"
#include "command.h"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

MessageHandler::MessageHandler() {}

Command MessageHandler::readMessage(const Connection& conn) const{
	stringstream ss;
	string name;
	vector<Argument> args;
	string arg;

	Protocol::a cmd_id = static_cast<Protocol::a>(conn.read());

	//cout << "command start: " << Protocol::map(cmd_id) << endl;

	unsigned char par_type;
	Protocol::a par_enum;

	while ((par_type = conn.read())){
		par_enum = static_cast<Protocol::a>(par_type);
		//cout << "	" << Protocol::map(par_enum) << endl;
		Argument arg(par_enum);
		//cout << "read " << Protocol::map(par_enum) << endl;
		if (par_type == Protocol::ANS_END || par_type == Protocol::COM_END){
			//cout << "	end of command byte received" << Protocol::map(par_enum) << endl;
			break;
		} else if (par_type == Protocol::PAR_NUM){
			arg = Argument(readInt(conn));
			//cout << "number parameter received" << arg.int_val << endl;
		} else if (par_type == Protocol::PAR_STRING){
			int length = readInt(conn);
			//cout << "	" << length << endl;
			stringstream ss;
			for (int i = 0; i < length; ++i){
				ss << conn.read();
				//cout << "read char" << endl;
			}
			arg = Argument(ss.str());
			//cout << "string parameter received" << arg.str_val << endl;
		}
		args.push_back(arg);
	}
	Command result(cmd_id, args);
	//cout << "	Bytes converted into command: " << result << endl;
	return result;
}

void MessageHandler::writeMessage(const Connection& conn, const Command cmd) const{
	char end = (cmd.id < 10) ? Protocol::COM_END : Protocol::ANS_END;

	conn.write(cmd.id);
	for (Argument arg : cmd.args){
		conn.write(arg.type);
		if (arg.type == Protocol::PAR_NUM){
			writeInt(conn, arg.int_val);
		} else if (arg.type == Protocol::PAR_STRING){
			writeString(conn, arg.str_val);
		}
	}
	conn.write(end);
}

void MessageHandler::writeInt(const Connection& conn, int value){
	conn.write((value >> 24) & 0xFF);
	conn.write((value >> 16) & 0xFF);
	conn.write((value >> 8) & 0xFF);
	conn.write(value & 0xFF);
}

int MessageHandler::readInt(const Connection& conn){
	unsigned char byte1 = conn.read();
	unsigned char byte2 = conn.read();
	unsigned char byte3 = conn.read();
	unsigned char byte4 = conn.read();
	return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

void MessageHandler::writeString(const Connection& conn, const string& value){
	writeInt(conn, value.size());
	for (const char& c : value) {
		conn.write(c);
	}
}

string MessageHandler::readString(const Connection& conn){
	int length = conn.read();
	stringstream ss;
	for (int i = 0; i < length; ++i){
		ss << conn.read();
	}
	string s;
	ss >> s;
	return s;
}
