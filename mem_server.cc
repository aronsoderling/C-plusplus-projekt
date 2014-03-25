#include "mem_server.h"
#include "news.h"
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

MemServer::MemServer(int p) : NewsServer(p){}

vector<Group> MemServer::listGroups(){
	return groups;
}
bool MemServer::createGroup(string name){
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
vector<Article> MemServer::listArticles(int groupId){
	auto g = find_if(groups.begin(), groups.end(),
		[groupId] (const Group& g) { return g.getId() == groupId; } 
		);
	if(g != groups.end()){
		return g->getArticles();
	}else{
		throw GroupDoesNotExistException();
	}
}
void MemServer::createArticle(int groupId, string title, 
		string author, string text){
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
	
bool MemServer::existsGroup(int groupId){
	return find_if(groups.begin(), groups.end(),
		[groupId] (const Group& g) { return g.getId() == groupId; } 
		) != groups.end();
}

Article MemServer::getArticle(int groupId, int articleId){
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