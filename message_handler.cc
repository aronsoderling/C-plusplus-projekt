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
	vector<string> args;
	string arg;

	vector<char> bytes;
	unsigned char ch = conn.read();
	while (ch != Protocol::COM_END && ch != Protocol::ANS_END){
		bytes.push_back(ch);
		ch = conn.read();
	}
	return Command(bytes);

}

void MessageHandler::writeMessage(const Connection& conn, const Command cmd) const{
	char end = (cmd.id < 10) ? Protocol::COM_END : Protocol::ANS_END;

	conn.write(cmd.id);
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
