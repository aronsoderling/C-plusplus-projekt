#include "command.h"
#include "protocol.h"
#include <string>
#include <sstream>
#include <vector>

using namespace std;

Command::Command(string str){
	string res;
	stringstream ss;
	string name;

	ss << str;
	ss >> name;

	while (ss >> res){
		if (name == "list"){
			args.push_back(Argument(res));
		} else if (name == "something else"){

		}
	}
	
	if (name == "list"){
		if (args.size() == 0){
			id = Protocol::COM_LIST_NG;
		} else if (args.size() == 1){
			id = Protocol::COM_LIST_ART;
		}
	} else if (name == "create"){

	} else{
		throw InvalidCommandException();
		cout << "Unknown command" << endl;
	}

	cout << "name: '" << name << "', id: " << Protocol::map(id)
			<< " args_length: " << args.size() << endl;
}

ostream& operator<<(ostream& os, const Command& cmd){
	os << Protocol::map(cmd.id);
	for (Argument a : cmd.args){
		if (a.type == Protocol::PAR_NUM) os << ' ' << a.int_val;
		else if (a.type == Protocol::PAR_STRING) os << ' ' << a.str_val;
		else os << " [unknown parameter type]";
	}
	return os;
}
