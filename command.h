#ifndef COMMAND_H
#define COMMAND_H

#include "protocol.h"
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <exception>

struct Argument{
	Argument(Protocol::a t) : type(t){};
	Argument(std::string v) : str_val(v), type(Protocol::PAR_STRING){};
	Argument(int v) : int_val(v), type(Protocol::PAR_NUM){};
	Protocol::a type;
	std::string str_val;
	int int_val;
};

class InvalidCommandException : public std::exception{};

class Command{
public:
	Command(std::string str);
	Command(enum Protocol::a i) : id(i) {};
	Command(enum Protocol::a i, std::vector<Argument> a) : id(i), args(a) {};
	enum Protocol::a id;
	std::vector<Argument> args;
};

std::ostream& operator<<(std::ostream& os, const Command& cmd);

#endif