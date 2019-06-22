#include <fstream>
#include <sstream>
#include <string.h>
#include "Row.h"
#include "Table.h"
#include "Database.h"
#include "DatabaseMap.h"
#include "Command.h"
#include "clause_deal.h"

bool starting=false;

int cutstring(char* info,string* word){
	char *pdeal;//用于处理命令的指针 
	int how_many_word=0;//word数组的元素个数
	pdeal = strtok(info," \t");//用空格等把指令分成一个一个的“单词”（逗号括号和换行都被剥掉了） 
	while(pdeal)
	{
		word[how_many_word++]=pdeal;
		pdeal=strtok(NULL," \t"); //之前的字符串被破坏了，而且第二次使用开头的指针会变成NULL 
	}
	return how_many_word;
}

void start(){
	starting=true;
	fstream check;
	check.open("1sqltql.txt",ios::in);
	if(check){
		char cinfo[1200];
		while(check.getline(cinfo,1100)){
			string info=cinfo;
			string word[2];
			word[0]=info.substr(0,info.find(' '));
			word[1]=info.substr(info.find(' ')+1);
			DB.CreateDatabase(word[0]);
			DB.UseDatabase(word[0]);
			string tablename=word[1];
			string tableFileName="1"+word[0]+"+"+tablename+".txt";
			fstream fin(tableFileName,ios::in);
			char Tinfo[1200];
			fin.getline(Tinfo,1100);
			vector<Attribute> attr;
			string key;
			while(fin.getline(Tinfo,1100)){
				//cout<<"*"<<Tinfo<<"*\n";
				
				if(!strcmp(Tinfo, "+++++")){
					break; 
				} 
				string Tword[100];//把每条命令抽成单词 
				int how_many_word1=cutstring(Tinfo,Tword);
				
				for(int i=0;i<how_many_word1;i++){
					//cout<<"w "<<Tword[i]<<endl;
				}
				
				Attribute Tattr;
				Tattr.name=Tword[0];
				if(Tword[1]=="int(11)"){
					Tattr.type="int";
				}
				else if(Tword[1]=="char(1)"){
					Tattr.type="char";
				}
				else{
					Tattr.type=Tword[1];
				}
				
				if(Tword[2]=="NO"){
					Tattr.Null=false;
				}
				if(Tword[3]=="PRI"){
					Tattr.Key=true;
					key=Tword[0];
				}
				attr.push_back(Tattr);
				
			}
			DB.current_db->CreateTable(tablename,attr,key);
			fin.getline(Tinfo,1100);
			vector<string> attr_list;
			string Aword[100];
			int how_many_attr=cutstring(Tinfo,Aword);
			for(int i=0;i<how_many_attr;i++){
				attr_list.push_back(Aword[i]);
			}
			while(fin.getline(Tinfo,1100)){
				vector<string> value_list;
				string Vword[100];
				int how_many_value=cutstring(Tinfo,Vword);
				for(int i=0;i<how_many_value;i++){
					value_list.push_back(Vword[i]);
				}
				DB.current_db->table_list[tablename].insert(attr_list,value_list);
			}
			fin.close();
		}
		check.close();
	}
	starting=false;
}

DatabaseMap DB;       
using namespace std;
int main()
{
//	start();
	//cout<<"start"<<endl;
	std::string command;
	char cmd[1200];
	while (cin.getline( cmd,1100, ';')){
		command=cmd;
	//	cout<<"comd="<<cmd<<endl;
	/*	if(command == "\n"){
			break;
		}*/
		Command a(command);
		a.operate();
		clause_deal(cmd,command);
	//	cout<<cmd<<endl;
	} 
}

