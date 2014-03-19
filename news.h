#ifndef NEWS_H
#define NEWS_H

#include <string>

class Article {
public:
	Article(int i, std::string ti, std::string a, std::string te) : 
		id(i), title(ti), author(a), text(te) {};
	int getId(){ return id; };
	std::string getTitle(){ return title; };
	std::string getAuthor(){ return author; };
private:
	int id;
	std::string title;
	std::string author;
	std::string text;
};


class Group {
public:
	Group(int i, std::string n) : id(i), name(n){};
	int getId(){ return id; };
	std::string getName(){ return name; };
private:
	int id;
	std::string name;
};

#endif
