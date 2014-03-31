#include "db_server.h"
#include "news.h"
#include <string>
#include <vector>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fstream>

using namespace std;

DbServer::DbServer(int p) : MemServer(p){
	string line;
	mkdir("groups", 0777);
	ifstream myfile("group_info");
	if (myfile.is_open()){
		getline(myfile, line);
		next_group_id = strtol(line.c_str(), 0, 10); // stoi(argv[2]); does not work in cygwin
	}
	myfile.close();
	readGroups();
}


bool DbServer::createGroup(string name){
	if (MemServer::createGroup(name)){
		stringstream ss;
		ss << "groups/" << next_group_id << " " << name;
		mkdir(ss.str().c_str(), 0777);
		return true;
	}
	return false;
}

vector<Group> DbServer::listGroups(){
	return MemServer::listGroups();
}

bool DbServer::deleteGroup(int groupId){
	vector<Article> arts = MemServer::listArticles(groupId);
	if (MemServer::deleteGroup(groupId)){
		for (Article a : arts){
			deleteArticle(groupId, a.getId());
		}
		return true;
	}
	return false;
}
vector<Article> DbServer::listArticles(int groupId){
	return MemServer::listArticles(groupId);
}

void DbServer::createArticle(int groupId, string title, 
		string author, string text){
	MemServer::createArticle(groupId, title, author, text);
	/*stringstream ss;
	ss << "groups/" << groupId << "/" << ;
	ifstream myfile(ss.str());
	if (myfile.is_open()){
		getline(myfile, line);
		next_group_id = strtol(line.c_str(), 0, 10); // stoi(argv[2]); does not work in cygwin
	}
	myfile.close();*/
}

void DbServer::deleteArticle(int groupId, int articleId){
	return MemServer::deleteArticle(groupId, articleId);
}
	
bool DbServer::existsGroup(int groupId){
	return MemServer::existsGroup(groupId);
}

Article DbServer::getArticle(int groupId, int articleId){
	return MemServer::getArticle(groupId, articleId);
}

int DbServer::newGroupId(){
	MemServer::newGroupId();
	ofstream myfile;
	myfile.open("group_info");
	myfile << next_group_id;
	myfile.close();
	return next_group_id;
}

void DbServer::readGroups(){
	vector<string> gr = readDir("groups");
	groups.clear();
	for (string s : gr){
		stringstream ss;
		ss << s;
		int nr;
		string name;
		ss >> nr;
		ss >> name;
		groups.push_back(Group(nr, name));
	}
}
vector<string> DbServer::readDir(string dir_name){
	vector<string> result;
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(dir_name.c_str())) != NULL) {
		/* print all the files and directories within directory */
		int i = 0;
		while ((ent = readdir(dir)) != NULL) {
			if (i > 1)
				result.push_back(ent->d_name);
			++i;
		}
		closedir(dir);
	} else {
		/* could not open directory */
		perror("Could not open directory");
		exit(1);
	}
	return result;
}