#include <iostream>
#include <string>
#include "mf.h"
#include <ctime>
#include <iomanip>

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
	if(Finds(word,"ADDDATE",how_many_word)!=-1)
	{
	this->AddDate();return 1;
	}
	if(Finds(word,"ADDTIME",how_many_word)!=-1)
	{
	this->AddTime();return 1;
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
void MathFunction::AddDate()
{
	string str = word[2].substr(1, word[2].length() - 2);
	int pos1 = str.find('-');
	int pos2 = str.substr(pos1+1).find('-') + pos1 + 1;
	if(pos1 == -1 || pos2 == pos1 || pos2 == str.length() - 1) {
		cout<<"0DATE\n";
		return;
	}
	for(int i=0;i<pos1;i++) {
		if(!isdigit(str[i])) {
			cout<<"0DATE\n";
			return;
		}
	}
	for(int i=pos1+1;i<pos2;i++) {
		if(!isdigit(str[i])) {
			cout<<"0DATE\n";
			return;
		}
	}
	for(int i=pos2+1;i<str.length();i++) {
		if(!isdigit(str[i])) {
			cout<<str[i]<<endl;
			cout<<"0DATE\n";
			return;
		}
	}
	int year = stoi(str.substr(0, pos1));
	int month = stoi(str.substr(pos1 + 1, pos2));
	int day = stoi(str.substr(pos2 + 1));
	struct tm t1 = { 0 };
	t1.tm_year = year - 1900; t1.tm_mon = month; t1.tm_mday = day + stoi(word[4]);
	mktime(&t1);
	cout<<t1.tm_year + 1900<<'-'<<setw(2)<<setfill('0')<<t1.tm_mon<<'-'<<setw(2)<<setfill('0')<<t1.tm_mday<<endl;
}
void MathFunction::AddTime()
{
	int pos1 = word[2].find('-');
	int pos2 = word[2].substr(pos1+1).find('-') + pos1 + 1;
	int pos3 = word[3].find(':');
	int pos4 = word[3].substr(pos3+1).find(':') + pos3 + 1;
	if(pos1 == -1 || pos2 == pos1 || pos2 == word[2].length() - 1) {
		cout<<"0TIME\n";
		return;
	}
	if(pos3 == -1 || pos4 == pos3 || pos4 == word[3].length() - 1) {
		cout<<"0TIME\n";
		return;
	}
	for(int i=0;i<pos1;i++) {
		if(!isdigit(word[2][i])) {
			cout<<"0TIME\n";
			return;
		}
	}
	for(int i=pos1+1;i<pos2;i++) {
		if(!isdigit(word[2][i])) {
			cout<<"0TIME\n";
			return;
		}
	}
	for(int i=pos2+1;i<word[2].length();i++) {
		if(!isdigit(word[2][i])) {
			cout<<"0TIME\n";
			return;
		}
	}
	for(int i=0;i<pos3;i++) {
		if(!isdigit(word[3][i])) {
			cout<<"0TIME\n";
			return;
		}
	}
	for(int i=pos3+1;i<pos4;i++) {
		if(!isdigit(word[3][i])) {
			cout<<"0TIME\n";
			return;
		}
	}
	for(int i=pos4+1;i<word[3].length();i++) {
		if(!isdigit(word[3][i])) {
			cout<<"0TIME\n";
			return;
		}
	}
	int year = stoi(word[2].substr(0, pos1));
	int month = stoi(word[2].substr(pos1 + 1, pos2));
	int day = stoi(word[2].substr(pos2 + 1));
	int hour = stoi(word[3].substr(0, pos3));
	int minute = stoi(word[3].substr(pos3 + 1, pos4));
	int second = stoi(word[3].substr(pos4 + 1));
	struct tm t1 = { 0 };
	t1.tm_year = year - 1900; t1.tm_mon = month; t1.tm_mday = day;
	t1.tm_hour = hour; t1.tm_min = minute; t1.tm_sec = second + stoi(word[4]);
	mktime(&t1);
	cout<<t1.tm_year + 1900<<'-'<<setw(2)<<setfill('0')<<t1.tm_mon<<'-'<<setw(2)<<setfill('0')<<t1.tm_mday<<' '<<setw(2)<<setfill('0')<<t1.tm_hour<<':'<<setw(2)<<setfill('0')<<t1.tm_min<<':'<<setw(2)<<setfill('0')<<t1.tm_sec<<endl;
}
