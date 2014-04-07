#ifndef MEM_SERVER_H
#define MEM_SERVER_H

#include "news_server.h"
#include "news.h"
#include <string>
#include <vector>

class MemServer : public NewsServer {
public:
	MemServer(int port);
	std::vector<Group> listGroups() const;
	bool createGroup(const std::string& name);
	bool deleteGroup(int id);
	bool existsGroup(int id) const;
	std::vector<Article> listArticles(int groupId) const;
	void createArticle(int groupId, const std::string& title,
		const std::string& author, const std::string& text);
	void deleteArticle(int groupId, int articleId);
	Article getArticle(int groupId, int articleId) const;
	int newGroupId();
protected:
	std::vector<Group> groups;
	int next_group_id = 1;
};

#endif
