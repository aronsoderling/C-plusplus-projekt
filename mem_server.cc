#include "mem_server.h"
#include "news.h"
#include <string>
#include <vector>

using namespace std;

MemServer::MemServer(int p) : NewsServer(p){}

vector<Group> MemServer::listGroups(){
	return groups;
}
bool MemServer::createGroup(string name){
	groups.push_back(Group(groups.size()+1, name));
}
bool MemServer::deleteGroup(string name){

}
vector<Article> MemServer::listArticles(int groupId){
	return vector<Article>();
}
bool MemServer::createArticle(int groupId, string title, 
		string author, string text){

}
bool MemServer::deleteArticle(int groupId, string articleId){

}
Article MemServer::getArticle(int groupId, string articleId){
	return Article(0, "", "", "");
}
