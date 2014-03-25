#ifndef MEM_SERVER_H
#define MEM_SERVER_H

#include "news_server.h"
#include "news.h"
#include <string>
#include <vector>

class MemServer : public NewsServer {
public:
	MemServer(int port);
	std::vector<Group> listGroups();
	bool createGroup(std::string name);
	bool deleteGroup(std::string name);
	std::vector<Article> listArticles(int groupId);
	bool createArticle(int groupId, std::string title, 
		std::string author, std::string text);
	bool deleteArticle(int groupId, std::string articleId);
	Article getArticle(int groupId, std::string articleId);
private:
	std::vector<Group> groups;
};

#endif
