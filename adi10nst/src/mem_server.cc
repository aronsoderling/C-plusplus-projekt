#include "mem_server.h"
#include "news.h"
#include <string>
#include <vector>
#include <algorithm>
#include <memory>

#include <iostream>
#include <stdexcept>
#include <memory>
#include <stdlib.h> // strtol

using namespace std;

MemServer::MemServer(int p) : NewsServer(p){}

vector<Group> MemServer::listGroups() const{
	return groups;
}
bool MemServer::createGroup(const string& name){
	if (groups.empty()){
		groups.push_back(Group(newGroupId(), name));
		return true;
	}
	if(find_if(groups.begin(), groups.end(), 
		[name](const Group& g) {return g.getName() == name;})
		!= groups.end()){
		return false;
	}else{
		groups.push_back(Group(newGroupId(), name));
		return true;
	}
}
bool MemServer::deleteGroup(int groupId){
	auto pos = find_if(groups.begin(), groups.end(),
		[groupId] (const Group& g) { return g.getId() == groupId; } );
	if(pos != groups.end()){
		groups.erase(pos);
		return true;
	}else{
		return false;
	}
}
vector<Article> MemServer::listArticles(int groupId) const{
	auto g = find_if(groups.begin(), groups.end(),
		[groupId] (const Group& g) { return g.getId() == groupId; } 
		);
	if(g != groups.end()){
		return g->getArticles();
	}else{
		throw GroupDoesNotExistException();
	}
}
void MemServer::createArticle(int groupId, const string& title, 
		const string& author, const string& text){
	auto g = find_if(groups.begin(), groups.end(),
		[groupId] (const Group& g) { return g.getId() == groupId; } 
		);
	if(g != groups.end()){
		g->addArticle(Article(g->newArticleId(), title, author, text));
	}else{
		throw GroupDoesNotExistException();
	}
}
void MemServer::deleteArticle(int groupId, int articleId){
	auto g = find_if(groups.begin(), groups.end(),
		[groupId] (const Group& g) { return g.getId() == groupId; } 
		);
	if(g != groups.end()){
		g->deleteArticle(articleId);
	}else{
		throw GroupDoesNotExistException();
	}
}
	
bool MemServer::existsGroup(int groupId) const{
	return find_if(groups.begin(), groups.end(),
		[groupId] (const Group& g) { return g.getId() == groupId; } 
		) != groups.end();
}

Article MemServer::getArticle(int groupId, int articleId) const{
	auto g = find_if(groups.begin(), groups.end(),
		[groupId] (const Group& g) { return g.getId() == groupId; } 
		);
	if(g != groups.end()){
		return g->getArticle(articleId);
	} else{
		throw GroupDoesNotExistException();
	}
}
int MemServer::newGroupId(){
	return next_group_id++;
}

int main(int argc, char* argv[]){
	if (argc != 2) {
		cerr << "Usage: mem_server <port-number>" << endl;
		return 1;
	}

	int port = -1;
	try {
		port = strtol(argv[1], 0, 10); // stoi(argv[2]); does not work in cygwin
	} catch (exception& e) {
		cerr << "Wrong port number. " << e.what() << endl;
		return 1;
	}

	unique_ptr<NewsServer> s = unique_ptr<NewsServer>(new MemServer(port));
	s->run();
}
