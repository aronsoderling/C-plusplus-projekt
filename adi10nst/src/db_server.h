#ifndef DB_SERVER_H
#define DB_SERVER_H

#include "mem_server.h"
#include "news.h"
#include <string>
#include <vector>

class DbServer : public NewsServer {
public:
	DbServer(int port);
	std::vector<Group> listGroups() const;
	bool createGroup(const std::string& name);
	bool deleteGroup(int id);
	bool existsGroup(int id) const;
	bool existsGroup(const std::string& name) const;
	std::vector<Article> listArticles(int groupId) const;
	void createArticle(int groupId, const std::string& title, 
		const std::string& author, const std::string& text);
	void deleteArticle(int groupId, int articleId);
	Article getArticle(int groupId, int articleId) const;
	int newGroupId();
	Group getGroup(int groupId) const;
	std::vector<Group> readGroups() const;
	std::vector<std::string> readDir(std::string dir_name);
	void deleteDir(const std::string& dir);
	void writeGroups(const std::vector<Group>& groups);
	void writeArticles(int groupId, int next_article_id, const std::vector<Article>& articles);
	int getNextArticleId(int groupId) const;
protected:
	mutable int next_group_id = 0;
};

#endif
