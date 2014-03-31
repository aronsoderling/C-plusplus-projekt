#ifndef MEM_SERVER_H
#define MEM_SERVER_H

#include "news_server.h"
#include "news.h"
#include <string>
#include <vector>

class MemServer : public NewsServer {
public:
	MemServer(int port);
	virtual std::vector<Group> listGroups();
	virtual bool createGroup(std::string name);
	virtual bool deleteGroup(int id);
	virtual bool existsGroup(int id);
	virtual std::vector<Article> listArticles(int groupId);
	virtual void createArticle(int groupId, std::string title,
		std::string author, std::string text);
	virtual void deleteArticle(int groupId, int articleId);
	virtual Article getArticle(int groupId, int articleId);
	virtual int newGroupId();
protected:
	std::vector<Group> groups;
	int next_group_id = 1;
};

#endif
