#include "command.h"
#include "protocol.h"
#include <string>
#include <sstream>
#include <vector>

using namespace std;

Command::Command(string str){
	string res_str;
	int res_int;
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
	case Protocol::COM_CREATE_NG:
		ss >> res_str;
		args.push_back(Argument(res_str));
		break;
	case Protocol::COM_DELETE_NG:
	case Protocol::COM_LIST_ART:
		ss >> res_int;
		args.push_back(Argument(res_int));
		break;
	case Protocol::COM_CREATE_ART:
		ss >> res_int;
		args.push_back(Argument(res_int));
		ss >> res_str;
		args.push_back(Argument(res_str));
		ss >> res_str;
		args.push_back(Argument(res_str));
		ss >> res_str;
		args.push_back(Argument(res_str));
		break;
	case Protocol::COM_DELETE_ART:
	case Protocol::COM_GET_ART:
		ss >> res_int;
		args.push_back(Argument(res_int));
		ss >> res_int;
		args.push_back(Argument(res_int));
		break;
	}

	cout << "name: '" << name << "', id: " << Protocol::map(id)
			<< " args_length: " << args.size() << endl;
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
