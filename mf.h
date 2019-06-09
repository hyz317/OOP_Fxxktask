#pragma once
#include <string>
#include <iostream>
#include <ctime>

using namespace std;

class MathFunction{
	private:
	string* word;
	int how_many_word;
	public:
	MathFunction(string *s,int wordnum)
	{
		word=s;
		how_many_word=wordnum;
	}
	~MathFunction(){};
	bool Deal();
	void Locate();
	void Lcase();
	void Ucase();
	void Space();
	void Left();
	void Strcmp(); 
	void AddDate();
	void AddTime();
	void plus();
	void minus();
	void times();
	void divide();
	void DIV();
	void mod();
};

int Finds(string *word,string keyword,int how_many_word);
void tolower(string& x);
void toupper(string& x);
