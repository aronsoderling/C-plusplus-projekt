#include "mem_server.h"
#include "news.h"
#include <string>
#include <vector>

using namespace std;

MemServer::MemServer(int p) : NewsServer(p){}

vector<Group> MemServer::listGroups(){
	return vector<Group>();
}
void MemServer::createGroup(string name){

}
void MemServer::deleteGroup(string name){

}
vector<Article> MemServer::listArticles(int groupId){
	return vector<Article>();
}
void MemServer::createArticle(int groupId, string title, 
		string author, string text){

}
void MemServer::deleteArticle(int groupId, string articleId){

}
Article MemServer::getArticle(int groupId, string articleId){
	return Article(0, "", "", "");
}
