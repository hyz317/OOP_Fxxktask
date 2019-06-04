#include<iostream>
#include "mf.h" 
using namespace std;
void clause_deal(char* cmd,string command);
void Count(string *word);
void Group_by(string*,int, string s = "");
int Find(string*,string,int);
void NewSelect(string*, int, string, string order_by_attr = "");
string GetOrderbyType(string table_name, string order_by_attr);
void Output(string* word,string command);
void Input(string* word,int how_many_word);
void Union(string* word,int how_many_word);
