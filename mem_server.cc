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
	if(find(groups.begin(), groups.end(), 
		[name](const Group& g) {return g.getName() == name;})
		!= groups.end()){
	
		groups.push_back(Group(groups.size()+1, name));
		return true;
	}else{
		return false;
	}
}
bool MemServer::deleteGroup(int groupId){
	auto pos = find(groups.begin(), groups.end(),
		[groupId] (const Group& g) { return g.getId() == groupId; } );
	if(pos != groups.end()){
		groups.erase(pos);
		return true;
	}else{
		return false;
	}
}
vector<Article> MemServer::listArticles(int groupId){
	auto g = find(groups.begin(), groups.end(),
		[groupId] (const Group& g) { return g.getId() == groupId; } 
		);
	if(g != groups.end()){
		return g->getArticles();
	}else{
		//throw något
	}
}
bool MemServer::createArticle(int groupId, string title, 
		string author, string text){
	auto g = find(groups.begin(), groups.end(),
		[groupId] (const Group& g) { return g.getId() == groupId; } 
		);
	if(g != groups.end()){
		g->addArticle(Article(, title, author, text));
	}else{
		//throw något
	}
}
bool MemServer::deleteArticle(int groupId, int articleId){
	auto g = find(groups.begin(), groups.end(),
		[groupId] (const Group& g) { return g.getId() == groupId; } 
		);
	if(g != groups.end()){
		if(g->deleteArticle(articleId)){
			return true;
		}else{
			return false;
		}
	}else{
		return false;
	}
}
	
bool MemServer::existsGroup(int groupId){
	return find(groups.begin(), groups.end(),
		[groupId] (const Group& g) { return g.getId() == groupId; } 
		) != groups.end();
}

Article MemServer::getArticle(int groupId, int articleId){
	auto g = find(groups.begin(), groups.end(),
		[groupId] (const Group& g) { return g.getId() == groupId; } 
		);
	if(g != groups.end()){
		return g->getArticle(articleId);
	}
}
