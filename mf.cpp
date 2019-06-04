#include <iostream>
#include <string>
#include "mf.h"
using namespace std;

int Finds(string *word,string keyword,int how_many_word) // 从 string 数组中找正确的下标 
{
	for(int i=0;i<how_many_word;i++)
	{
		if(word[i]==keyword)return i;
	}
	return -1;
}
bool MathFunction::Deal()
{
	if(Finds(word,"LOCATE",how_many_word)!=-1)
	{
	this->Locate();return 1; 	
	}
	if(Finds(word,"STRCMP",how_many_word)!=-1)
	{
	this->Strcmp();return 1; 
	}
	if(Finds(word,"LCASE",how_many_word)!=-1)
	{
	this->Lcase();return 1; 
	}
	if(Finds(word,"UCASE",how_many_word)!=-1)
	{
	this->Ucase();return 1; 
	}
	if(Finds(word,"SPACE",how_many_word)!=-1)
	{
	this->Space();return 1;
	}
	if(Finds(word,"LEFT",how_many_word)!=-1)
	{
	this->Left();return 1;
	}
	return 0;
}
void tolower(string& x)
{
	for(int i=0;i<x.length();i++)
	{
		if(x[i]>='A'&&x[i]<='Z')x[i]+=32;
	}
}
void toupper(string& x)
{
	for(int i=0;i<x.length();i++)
	{
		if(x[i]>='a'&&x[i]<='z')x[i]-=32;
	}
}
void MathFunction::Space()
{
	int t=stoi(word[2]);
	while(t--)
		cout<<" ";
	cout<<endl;
}
void MathFunction::Lcase()
{
	tolower(word[2]);
	cout<<word[2]<<endl;
}
void MathFunction::Ucase()
{
	toupper(word[2]);
	cout<<word[2]<<endl;
}
void MathFunction::Strcmp()
{
	if(word[2]<word[3])cout<<-1<<endl;
	if(word[2]==word[3])cout<<0<<endl;
	if(word[2]>word[3])cout<<1<<endl; 
}
void MathFunction::Locate()
{
	cout<<word[3].find(word[2])+1<<endl;
}
void MathFunction::Left()
{
	int t=stoi(word[3]);
	for(int i=0;i<t;i++)cout<<word[2][i];
	cout<<endl;
}
