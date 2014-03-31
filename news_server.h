#ifndef NEWS_SERVER_H
#define NEWS_SERVER_H

#include "news.h"
#include <string>
#include <vector>
#include "message_handler.h"
#include "command.h"

class NewsServer {
public:
	virtual std::vector<Group> listGroups() = 0;
	virtual bool createGroup(std::string name);
	virtual bool deleteGroup(int id) = 0;
	virtual bool existsGroup(int id) = 0;
	virtual std::vector<Article> listArticles(int groupId) = 0;
	virtual void createArticle(int groupId, std::string title, 
		std::string author, std::string text) = 0;
	virtual void deleteArticle(int groupId, int articleId) = 0;
	virtual Article getArticle(int groupId, int articleId) = 0;
	void run();
	Command executeCommand(Command c);
protected:
	NewsServer(int p) : port(p){};
	int port;
};

#endif
