#include "command.h"
#include "protocol.h"
#include <string>
#include <sstream>
#include <vector>

using namespace std;

Command::Command(string str){
	stringstream ss;
	string name;

	ss << str;
	ss >> name;

	if (name == "listg"){
		id = Protocol::COM_LIST_NG;
	} else if (name == "createg"){
		id = Protocol::COM_CREATE_NG;
	} else if (name == "deleteg"){
		id = Protocol::COM_DELETE_NG;
	} else if (name == "lista"){
		id = Protocol::COM_LIST_ART;
	} else if (name == "createa"){
		id = Protocol::COM_CREATE_ART;
	} else if (name == "deletea"){
		id = Protocol::COM_DELETE_ART;
	} else if (name == "reada"){
		id = Protocol::COM_GET_ART;
	} else{
		throw InvalidCommandException();
	}

	switch(id){
	case Protocol::COM_CREATE_NG: readStr(ss, args); break;
	case Protocol::COM_DELETE_NG:
	case Protocol::COM_LIST_ART: readInt(ss, args); break;
	case Protocol::COM_CREATE_ART:
		readInt(ss, args);
		readStr(ss, args);
		readStr(ss, args);
		readStr(ss, args);
		break;
	case Protocol::COM_DELETE_ART:
	case Protocol::COM_GET_ART:
		readInt(ss, args);
		readInt(ss, args);
		break;
	default: break;
	}
	

	//cout << "name: '" << name << "', id: " << Protocol::map(id)
	//		<< " args_length: " << args.size() << endl;
}

bool Command::ok(){
	return !(args[0].type == Protocol::ANS_NAK);
}
string Command::errMsg(){
	if (!ok()){
		switch (args[1].type){
		case Protocol::ERR_ART_DOES_NOT_EXIST: return "Article does not exist. ";
		case Protocol::ERR_NG_ALREADY_EXISTS: return "News group already exists. ";
		case Protocol::ERR_NG_DOES_NOT_EXIST: return "News group does not exist. ";
		default: break;
		}
	}
	return "";
}

ostream& operator<<(ostream& os, const Command& cmd){
	os << Protocol::map(cmd.id);
	for (Argument a : cmd.args){
		if (a.type == Protocol::PAR_NUM) os << ' ' << a.int_val;
		else if (a.type == Protocol::PAR_STRING) os << ' ' << a.str_val;
		else os << " " << Protocol::map(a.type);
	}
	return os;
}

void Command::readStr(stringstream& ss, vector<Argument>& args){
	string result;
	if (!ss.good()) throw InvalidCommandException();
	ss >> result;
	if (result == "") throw InvalidCommandException();
	args.push_back(Argument(result));
}

void Command::readInt(stringstream& ss, vector<Argument>& args){
	int result;
	if (!ss.good()) throw InvalidCommandException();
	ss >> result;
	if (result == 0) throw InvalidCommandException();
	args.push_back(Argument(result));
}