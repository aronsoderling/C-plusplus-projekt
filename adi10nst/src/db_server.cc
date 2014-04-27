#include "db_server.h"
#include "news.h"
#include <string>
#include <vector>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <memory>

using namespace std;

DbServer::DbServer(int p) : NewsServer(p){
	mkdir("groups", 0777);
}


bool DbServer::createGroup(const string& name){
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

vector<Group> DbServer::listGroups() const{
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

vector<Article> DbServer::listArticles(int groupId) const{
	Group g = getGroup(groupId);
	vector<Article> articles;

	if (g.getId() == 0)
		throw GroupDoesNotExistException();
	
	string line;
	stringstream filename;
	
	filename << "groups/" << groupId << "/article_info";
	ifstream infile(filename.str());
	if (!infile.is_open()){
		infile.close();
		cout << "Unable to read file: groups/" << groupId << "/article_info" << endl;
	}
	
	getline(infile, line);

	while (getline(infile, line, ' ')){
		int aId = strtol(line.c_str(), 0, 10);
		string aTitle;
		getline(infile, aTitle);
		articles.push_back(Article(aId, aTitle, "", ""));
	}
	infile.close();
	return articles;
}

void DbServer::createArticle(int groupId, const string& title, const string& author, const string& text){
	vector<Article> articles = listArticles(groupId);

	int next_article_id = getNextArticleId(groupId);
	Article a(++next_article_id, title, author, text);
	articles.push_back(a);

	stringstream ss;
	ss << "groups/" << groupId << "/" << a.getId();

	ofstream myfile;
	myfile.open(ss.str());
	myfile << author << endl;
	myfile << text;
	myfile.close();

	writeArticles(groupId, next_article_id, articles);
}

void DbServer::deleteArticle(int groupId, int articleId){
	vector<Article> articles = listArticles(groupId);

	auto pos = find_if(articles.begin(), articles.end(),
		[articleId](const Article& a) { return a.getId() == articleId; });
	if (pos != articles.end()){
		articles.erase(pos);
	} else{
		throw ArticleDoesNotExistException();
	}

	stringstream ss;
	ss << "groups/" << groupId << "/" << articleId;
	//deleteFile(ss.str());

	writeArticles(groupId, getNextArticleId(groupId), articles);
}
	
bool DbServer::existsGroup(int groupId) const{
	vector<Group> groups = listGroups();

	return find_if(groups.begin(), groups.end(),
		[groupId](const Group& g) { return g.getId() == groupId; }
	) != groups.end();
}

bool DbServer::existsGroup(const string& name) const{
	vector<Group> groups = listGroups();

	return find_if(groups.begin(), groups.end(),
		[name](const Group& g) { return g.getName() == name; }
	) != groups.end();
}

Group DbServer::getGroup(int groupId) const{
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

Article DbServer::getArticle(int groupId, int articleId) const{
	vector<Article> articles = listArticles(groupId);

	string line;
	//string text = "";
	string author;
	stringstream filename;
	filename << "groups/" << groupId << "/" << articleId;
	ifstream infile(filename.str());
	if (!infile.is_open()){
		infile.close();
		cout << "Unable to read file: groups/group_info" << endl;
		return Article();
	}
	
	getline(infile, author);
	std::string text((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
	infile.close();
	Article a;

	auto it = find_if(articles.begin(), articles.end(),
		[articleId](const Article& a) { return a.getId() == articleId; }
	);
	if (it != articles.end()){
		a = *it;
		//cout << text << endl;
		a.setAuthor(author);
		a.setText(text);
	}
	return a;
}

vector<string> DbServer::readDir(string dir_name){
	vector<string> result;
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
	return result;
}


void DbServer::deleteDir(const string& dir){
	vector<string> files = readDir(dir);
	for (string file : files){
		stringstream ss;
		ss << dir << "/" << file;
		if (remove(ss.str().c_str()) != 0){
			cerr << "Unable to remove " << ss.str();
		}
	}
	if (rmdir(dir.c_str()) != 0){
		cerr << "Unable to remove " << dir;
	}
}

void DbServer::writeGroups(const vector<Group>& groups){
	ofstream myfile;
	myfile.open("groups/group_info");
	myfile << next_group_id << endl;
	for (Group g : groups){
		myfile << g.getId() << " " << g.getName() << endl;
	}
	myfile.close();
}

void DbServer::writeArticles(int groupId, int next_article_id, const vector<Article>& articles){
	ofstream myfile;
	stringstream ss;
	ss << "groups/" << groupId << "/article_info";
	myfile.open(ss.str());
	myfile << next_article_id << endl;
	for (Article a : articles){
		myfile << a.getId() << " " << a.getTitle() << endl;
	}
	myfile.close();
}

int DbServer::getNextArticleId(int groupId) const{
	string line;
	stringstream filename;
	filename << "groups/" << groupId << "/article_info";
	ifstream infile(filename.str());
	if (!infile.is_open()){
		infile.close();
		cout << "Unable to read file: groups/" << groupId << "/article_info" << endl;
	}

	getline(infile, line);
	return strtol(line.c_str(), 0, 10); // stoi(argv[2]); does not work in cygwin
}

int main(int argc, char* argv[]){
	if (argc != 2) {
		cerr << "Usage: db_server <port-number>" << endl;
		return 1;
	}

	int port = -1;
	try {
		port = strtol(argv[1], 0, 10); // stoi(argv[2]); does not work in cygwin
	} catch (exception& e) {
		cerr << "Wrong port number. " << e.what() << endl;
		return 1;
	}

	unique_ptr<NewsServer> s = unique_ptr<NewsServer>(new DbServer(port));
	s->run();
}
