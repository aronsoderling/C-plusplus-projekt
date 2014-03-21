#include "message_handler.h"
#include "connection.h"
#include "protocol.h"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

MessageHandler::MessageHandler() {
}


Command MessageHandler::readMessage(const Connection& conn) const{
	stringstream ss;
	string name;
	vector<Argument> args;
	string arg;

	unsigned char cmd_byte = conn.read();
	unsigned char par_type = conn.read();

	unsigned char ch;
	while (ch = conn.read()){
		if (par_type == Protocol::ANS_END || par_type == Protocol::COM_END){
			break;
		} else if (par_type == Protocol::PAR_NUM){
			Argument arg( readInt(conn) );
			args.push_back(arg);
		} else if (par_type == Protocol::PAR_STRING){
			int length = conn.read();
			stringstream ss;
			for (int i = 0; i < length; ++i){
				ss << conn.read();
			}
			string s;
			ss >> s;
			Argument arg(s);
			args.push_back(arg);
		} else{
			cerr << "Unknown parameter" << endl;
		}
	}
	
	return Command(cmd_byte, args);
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
		} else{
			cerr << "Unknown argument type" << endl;
		}
	}
	conn.write(end);

	cout << "Wrote message" << endl;
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

void MessageHandler::writeString(const Connection& conn, string value){
	conn.write(value.size());
	for (char& c : value) {
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
