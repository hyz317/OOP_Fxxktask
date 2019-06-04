#pragma once
#include <string>
#include <iostream>

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
};
