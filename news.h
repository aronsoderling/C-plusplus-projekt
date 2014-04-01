#ifndef NEWS_H
#define NEWS_H

#include <string>
#include <vector>
#include <algorithm>

class ArticleDoesNotExistException : public std::exception{};
class GroupDoesNotExistException : public std::exception{};

class Article {
public:
	Article(int i, std::string ti, std::string a, std::string te) : 
		id(i), title(ti), author(a), text(te) {};
	int getId() const{ return id; };
	std::string getTitle(){ return title; };
	std::string getAuthor(){ return author; };
	std::string getText(){ return text; };
private:
	int id;
	std::string title;
	std::string author;
	std::string text;
};


class Group {
public:
	Group() : id(0), name(""){};
	Group(int i, std::string n) : id(i), name(n){};
	int getId() const{ return id; };
	std::string getName() const{ return name; };

	bool addArticle(const Article& a){
		if(find_if(articles.begin(), articles.end(), 
				[a](const Article& arg){ return a.getId() == arg.getId(); }) != articles.end()){
			return false;
		}else{
			articles.push_back(a);
			return true;
		}
	};
	void deleteArticle(int articleId) {
		auto a = find_if(articles.begin(), articles.end(), 
			[articleId](const Article& arg){ return arg.getId() == articleId;});
		if(a != articles.end()){
			articles.erase(a);
		}else{
			throw ArticleDoesNotExistException();
		}
	};
	std::vector<Article> getArticles() const{ return articles; };
	Article getArticle(int articleId) const{
		auto a = find_if(articles.begin(), articles.end(), 
			[articleId](const Article& arg){ return arg.getId() == articleId;});
		if(a != articles.end()){
			return *a;
		}else{
			throw ArticleDoesNotExistException();
		}
	};
	int newArticleId(){
		return next_article_id++;
	}
private:
	int id;
	std::string name;
	std::vector<Article> articles;
	int next_article_id = 1;
};

#endif
