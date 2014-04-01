#ifndef DB_SERVER_H
#define DB_SERVER_H

#include "mem_server.h"
#include "news.h"
#include <string>
#include <vector>

class DbServer : public NewsServer {
public:
	DbServer(int port);
	std::vector<Group> listGroups();
	bool createGroup(std::string name);
	bool deleteGroup(int id);
	bool existsGroup(int id);
	bool existsGroup(std::string name);
	std::vector<Article> listArticles(int groupId);
	void createArticle(int groupId, std::string title, 
		std::string author, std::string text);
	void deleteArticle(int groupId, int articleId);
	Article getArticle(int groupId, int articleId);
	int newGroupId();
	Group getGroup(int groupId);
	std::vector<Group> readGroups();
	std::vector<std::string> readDir(std::string dir_name);
	void deleteDir(std::string dir);
	void writeGroups(std::vector<Group> groups);
protected:
	int next_group_id = 1;
};

#endif
