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
	bool deleteGroup(int id);
	bool existsGroup(int id);
	std::vector<Article> listArticles(int groupId);
	bool createArticle(int groupId, std::string title, 
		std::string author, std::string text);
	bool deleteArticle(int groupId, int articleId);
	Article getArticle(int groupId, int articleId);
private:
	std::vector<Group> groups;
};

#endif
