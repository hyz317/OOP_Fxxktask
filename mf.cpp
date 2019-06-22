#include <iostream>
#include <string>
#include "mf.h"
#include <ctime>
#include <iomanip>
#include "DatabaseMap.h"
#include "Database.h"
#include "Table.h"
#include "Row.h"

using namespace std;
extern DatabaseMap DB;
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
	if(Finds(word,"LCASE",how_many_word)==1)
	{
	this->Lcase();return 1; 
	}
	if(Finds(word,"UCASE",how_many_word)==1)
	{
	this->Ucase();return 1; 
	}
	if(Finds(word,"SPACE",how_many_word)!=-1)
	{
	this->Space();return 1;
	}
	if(Finds(word,"LEFT",how_many_word)==1 && Finds(word,"JOIN",how_many_word)==-1)
	{
	this->Left();return 1;
	}
	if(Finds(word,"RIGHT",how_many_word)!=-1 && Finds(word,"JOIN",how_many_word)==-1)
	{
	this->Right();return 1;
	}
	if(Finds(word,"ADDDATE",how_many_word)!=-1)
	{
	this->AddDate();return 1;
	}
	if(Finds(word,"ADDTIME",how_many_word)!=-1)
	{
	this->AddTime();return 1;
	}
	if(word[1].find('+') != -1)
	{
	this->plus();return 1;
	}
	if(word[1].find('-') != -1)
	{
	this->minus();return 1;
	}
	if(word[1].find('*') != -1 && word[1] != "*")
	{
	this->times();return 1;
	}
	if(word[1].find('/') != -1)
	{
	this->divide();return 1;
	}
	if(Finds(word,"DIV",how_many_word)!=-1)
	{
	this->DIV();return 1;
	}
	if(Finds(word,"MOD",how_many_word)!=-1)
	{
	this->mod();return 1;
	}
	if(Finds(word,"AVG",how_many_word)!=-1)
	{
	this->Avg();return 1;
	}
	if(Finds(word,"MAX",how_many_word)!=-1)
	{
	this->Max();return 1;
	}
	if(Finds(word,"MIN",how_many_word)!=-1)
	{
	this->Min();return 1;
	}
	return 0;
}


void MathFunction::Right()
{
	int t=stoi(word[3]);
	for(int i=word[2].length()-t;i!=word[2].length();i++)cout<<word[2][i];
	cout<<endl;
}

void MathFunction::Avg()//SELECT AVG(Price) AS AveragePrice FROM Products;
{
//	cout<<"avg"<<endl;
	string col=word[2];
	string outputname=word[4];
	string tablename=word[6];
//	cout<<"col="<<col<<" "<<outputname<<" "<<tablename<<endl;
	Database* tmpdatabase=DB.current_db;
	//Table* thetable;
	long long total=0;
	int num=0;
	Table newtable;
	for(auto i:tmpdatabase->table_list)
	{
		if(i.first==tablename)
		{
			newtable=i.second;
		}
	}
	for(auto i=newtable.row_map.begin();i!=newtable.row_map.end();i++)
	{
		total+=stol(i->second.data[col]);
		num++;
	}
//	cout<<"!";
/*	
	for(auto i: tmpdatabase->table_list)
	{
		//cout<<i.first<<"&"<<tablename<<endl;
		if(i.first==tablename)
		{
			thetable=&i.second;
		}
	}
	int thesize=thetable->row_map.size();
	for(auto i = thetable->row_map.begin(); i != thetable->row_map.end(); ++i)
	{
		if(num==thesize)break;
		cout<<i->second.data[col]<<endl;
		total+=stol(i->second.data[col]);
		num++;
	}*/
	cout<<outputname<<endl;
	if(num!=0)cout<<total/num<<endl;
}

void MathFunction::Max()//SELECT MAX(Price) AS AveragePrice FROM Products;
{
	string col=word[2];
	string outputname=word[4];
	string tablename=word[6];
	Database* tmpdatabase=DB.current_db;
	int tmpmax=-9999999;
	int num=0;
	Table newtable;
	for(auto i:tmpdatabase->table_list)
	{
		if(i.first==tablename)
		{
			newtable=i.second;
		}
	}
	for(auto i=newtable.row_map.begin();i!=newtable.row_map.end();i++)
	{
		tmpmax=max(tmpmax,stoi(i->second.data[col]));
		num++;
	}
	cout<<outputname<<endl;
	cout<<tmpmax<<endl;
}

void MathFunction::Min()//SELECT MIN(Price) AS AveragePrice FROM Products;
{
	string col=word[2];
	string outputname=word[4];
	string tablename=word[6];
	Database* tmpdatabase=DB.current_db;
	int tmpmin=9999999;
	int num=0;
	Table newtable;
	for(auto i:tmpdatabase->table_list)
	{
		if(i.first==tablename)
		{
			newtable=i.second;
		}
	}
	for(auto i=newtable.row_map.begin();i!=newtable.row_map.end();i++)
	{
		tmpmin=min(tmpmin,stoi(i->second.data[col]));
		num++;
	}
	cout<<outputname<<endl;
	cout<<tmpmin<<endl;
}


void MathFunction::plus()
{
	cout<<word[1]<<endl;
	cout<<stod(word[1].substr(0, word[1].find('+'))) + stod(word[1].substr(word[1].find('+') + 1))<<endl;
}

void MathFunction::minus()
{
	cout<<word[1]<<endl;
	cout<<stod(word[1].substr(0, word[1].find('-'))) - stod(word[1].substr(word[1].find('-') + 1))<<endl;
}

void MathFunction::times()
{
	cout<<word[1]<<endl;
	cout<<stod(word[1].substr(0, word[1].find('*'))) * stod(word[1].substr(word[1].find('*') + 1))<<endl;
}

void MathFunction::divide()
{
	if(stod(word[1].substr(word[1].find('/')+1))==0) {
		cout<<"NULL\n";
		return;
	}
	cout<<word[1]<<endl;
	cout<<stod(word[1].substr(0, word[1].find('/'))) / stod(word[1].substr(word[1].find('/') + 1))<<endl;
}

void MathFunction::DIV()
{
	if(word[3] == "0") {
		cout<<"NULL\n";
		return;
	}
	cout<<word[1]<<' '<<word[2]<<' '<<word[3]<<endl;
	cout<<stod(word[1]) / stod(word[3])<<endl;
}

void MathFunction::mod()
{
	if(word[3] == "0") {
		cout<<"NULL\n";
		return;
	}
	cout<<word[1]<<' '<<word[2]<<' '<<word[3]<<endl;
	cout<<stoi(word[1]) % stoi(word[3])<<endl;
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
