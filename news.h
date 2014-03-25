#ifndef NEWS_H
#define NEWS_H

#include <string>

class Article {
public:
	Article(int i, std::string ti, std::string a, std::string te) : 
		id(i), title(ti), author(a), text(te) {};
	int getId() const{ return id; };
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
	int getId() const{ return id; };
	std::string getName() const{ return name; };
	bool addArticle(const Article& a){
		if(find(articles.begin(), articles.end(), 
			[a](const Article& arg){ return a.getId() == arg.getId();}) != articles.end()){
				articles.push_back(a);
				return true;
		}else{
			return false;
		}
	}
	bool deleteArticle(int articleId) {
		auto a = find(articles.begin(), articles.end(), 
			[articleId](const Article& arg){ return arg.getId() == articleId;});
		if(a != articles.end()){
				articles.erase(a);
				return true;
			}else{
				return false;
		}
	}
	vector<Article> getArticles() const{ return articles;}
	Article getArticle(int articleId) const{
		auto a = find(articles.begin(), articles.end(), 
			[articleId](const Article& arg){ return arg.getId() == articleId;});
		if(a != articles.end()){
			return *a;
		}else{
			//throw något 		
		}
	}
private:
	int id;
	std::string name;
	vector<Article> articles;
};

#endif
