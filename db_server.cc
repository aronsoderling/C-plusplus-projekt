#include "db_server.h"
#include "news.h"
#include <string>
#include <vector>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fstream>
#include <sstream>

using namespace std;

DbServer::DbServer(int p) : NewsServer(p){
	mkdir("groups", 0777);
}


bool DbServer::createGroup(string name){
	vector<Group> groups = listGroups();
	if (!existsGroup(name)){
		Group g(++next_group_id, name);
		groups.push_back(g);
		stringstream ss;
		ss << "groups/" << g.getId();
		mkdir(ss.str().c_str(), 0777);
		writeGroups(groups);
		return true;
	}
	return false;
}

vector<Group> DbServer::listGroups(){
	string line;
	vector<Group> groups;
	ifstream infile("groups/group_info");
	if (!infile.is_open()){
		infile.close();
		cout << "Unable to read file: groups/group_info" << endl;
		return groups;
	}
	getline(infile, line);
	next_group_id = strtol(line.c_str(), 0, 10); // stoi(argv[2]); does not work in cygwin

	while (getline(infile, line, ' ')){
		int gId = strtol(line.c_str(), 0, 10);
		string gName;
		getline(infile, gName);
		groups.push_back(Group(gId, gName));
	}
	infile.close();

	/*vector<string> gr = readDir("groups");
	
	for (string s : gr){
		stringstream ss;
		ss << s;
		int nr;
		string name;
		ss >> nr;
		ss >> name;
		groups.push_back(Group(nr, name));
	}*/
	return groups;
}

bool DbServer::deleteGroup(int groupId){
	Group g = getGroup(groupId);
	vector<Article> arts = g.getArticles();
	for (Article a : arts){
		deleteArticle(groupId, a.getId());
	}
	stringstream ss;
	ss << "groups/" << g.getId();
	deleteDir(ss.str());

	vector<Group> groups = listGroups();

	auto pos = find_if(groups.begin(), groups.end(),
		[groupId](const Group& g) { return g.getId() == groupId; });
	if (pos != groups.end()){
		groups.erase(pos);
		writeGroups(groups);
		return true;
	} else{
		return false;
	}
}

vector<Article> DbServer::listArticles(int groupId){
	Group g = getGroup(groupId);
	vector<Article> articles;

	if (g.getId() == 0)
		return articles;
	
	string line;
	stringstream filename;
	
	filename << "groups/" << groupId << "/article_info";
	ifstream infile(filename.str());
	if (!infile.is_open()){
		infile.close();
		cout << "Unable to read file: groups/" << groupId << "/article_info" << endl;
	}
	
	getline(infile, line);
	int next_article_id = strtol(line.c_str(), 0, 10); // stoi(argv[2]); does not work in cygwin
	
	while (getline(infile, line, ' ')){
		int aId = strtol(line.c_str(), 0, 10);
		string aTitle;
		getline(infile, aTitle);
		articles.push_back(Article(aId, aTitle, "", ""));
	}
	infile.close();
	return articles;
}

void DbServer::createArticle(int groupId, string title, string author, string text){
	/*Group g = getGroup(groupId);

	if (g.getId() != 0){
		Article a(g.newArticleId(), title, author, text);
		
		stringstream ss;
		ss << "groups/" << g.getFolderName() << "/" << a.getId();

		ofstream myfile;
		myfile.open(ss.str());
		myfile << title << ',' << author << ',' << text;
		myfile.close();
	} else{
		throw GroupDoesNotExistException();
	}*/
}

void DbServer::deleteArticle(int groupId, int articleId){
	/*Group g = getGroup(groupId);
	stringstream ss;
	ss << "groups/" << g.getFolderName() << "/" << articleId;
	deleteFile(ss.str());*/
}
	
bool DbServer::existsGroup(int groupId){
	vector<Group> groups = listGroups();

	return find_if(groups.begin(), groups.end(),
		[groupId](const Group& g) { return g.getId() == groupId; }
	) != groups.end();
}

bool DbServer::existsGroup(string name){
	vector<Group> groups = listGroups();

	return find_if(groups.begin(), groups.end(),
		[name](const Group& g) { return g.getName() == name; }
	) != groups.end();
}

Group DbServer::getGroup(int groupId){
	vector<Group> groups = listGroups();
	vector<Article> articles;
	Group g;
	auto it = find_if(groups.begin(), groups.end(),
		[groupId](const Group& g) { return g.getId() == groupId; }
	);
	if (it != groups.end()){
		g = *it;
	}
	return g;
}

Article DbServer::getArticle(int groupId, int articleId){
	/*Group g = getGroup(groupId);
	if (g.getId() != 0){
		
	} else{
		throw GroupDoesNotExistException();
	}*/
}

vector<string> DbServer::readDir(string dir_name){
	/*vector<string> result;
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(dir_name.c_str())) != NULL) {
		int i = 0;
		while ((ent = readdir(dir)) != NULL) {
			if (i > 1)
				result.push_back(ent->d_name);
			++i;
		}
		closedir(dir);
	} else {
		perror("Could not open directory");
		exit(1);
	}
	return result;*/
	return vector<string>();
}
void DbServer::deleteDir(string dir){

}

void DbServer::writeGroups(vector<Group> groups){
	ofstream myfile;
	myfile.open("groups/group_info");
	myfile << next_group_id << endl;
	for (Group g : groups){
		myfile << g.getId() << " " << g.getName() << endl;
	}
	myfile.close();
}