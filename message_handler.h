#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include "connection.h"
#include "protocol.h"
#include "command.h"
#include <string>
#include <iostream>

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
