#ifndef NEWS_SERVER_H
#define NEWS_SERVER_H

#include "news.h"
#include <string>
#include <vector>
#include "message_handler.h"

class NewsServer {
public:
	virtual std::vector<Group> listGroups() = 0;
	virtual void createGroup(std::string name) = 0;
	virtual void deleteGroup(std::string name) = 0;
	virtual std::vector<Article> listArticles(int groupId) = 0;
	virtual void createArticle(int groupId, std::string title, 
		std::string author, std::string text) = 0;
	virtual void deleteArticle(int groupId, std::string articleId) = 0;
	virtual Article getArticle(int groupId, std::string articleId) = 0;
	void run();
	Command executeCommand(Command c);
protected:
	NewsServer(int p) : port(p){};
	int port;
};

#endif
