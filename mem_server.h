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
	void createGroup(std::string name);
	void deleteGroup(std::string name);
	std::vector<Article> listArticles(int groupId);
	void createArticle(int groupId, std::string title, 
		std::string author, std::string text);
	void deleteArticle(int groupId, std::string articleId);
	Article getArticle(int groupId, std::string articleId);
private:
	std::vector<Group> groups;
};

#endif
