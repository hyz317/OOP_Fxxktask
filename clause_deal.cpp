#include "clause_deal.h"
#include "Database.h"
#include "DatabaseMap.h"
#include "Row.h"
#include <cstring>
#include <fstream>
#include <sstream>
extern DatabaseMap DB;
using namespace std;
void Select(std::stringstream& ss,bool foutput=false);

void clause_deal(char* cmd,string command)
{
//	char cmd[1020];
//	if(!cin.getline(cmd,1000,';'))break;//若指令终止，说明一条语句结束，则break
		string scmd=cmd;//做备份（之后要用strtok）
		string WhereString;
	//rr的clause tree可以当作附加功能，不用它的那个select里面的 
	/*	Wherenode *rootnode = nullptr;
		if(scmd.find("WHERE") != -1) {
			WhereString = scmd.substr(scmd.find("WHERE") + 6);
			rootnode = new Wherenode;
			rootnode->prev = nullptr;
		}*/ 
		cout<<__FUNCTION__<<__LINE__<<endl; 
		char *pdeal;//用于处理命令的指针 
		string word[100];//把每条命令抽成单词 
		int how_many_word=0;//word数组的元素个数
		pdeal = strtok(cmd,"' ,()\n");//用空格等把指令分成一个一个的“单词”（逗号括号和换行都被剥掉了） 
		while(pdeal)
		{
			word[how_many_word++]=pdeal;
			pdeal=strtok(NULL,"'\n ,()"); //之前的字符串被破坏了，而且第二次使用开头的指针会变成NULL 
		}
		if(word[1]=="COUNT")Count(word); //先没区分大小写 
		//把已经切分成“单词”的命令传入deal_系列函数中进行处理 
	//	deal_database(word,tmp_database,database);//如果把NULL传进去，只有通过返回指针才可以得到新指针 
	//	deal_datalist(word,tmp_database,how_many_word);
	//	deal_data(word,tmp_database,how_many_word,WhereString,rootnode);
		if(word[3]=="OUTFILE") {
			string OutFileName=word[4];
			fstream check;
			check.open(OutFileName,ios::in);
			if(check){
				cout<<"文件已存在！换一个名字吧！"<<endl;
			}
			else{
				check.close();
				cout<<"输出"<<endl;
				fstream fout;
				fout.open(OutFileName,ios::out);
				streambuf* ocb=cout.rdbuf();//origin cout buffer
				streambuf* fb=fout.rdbuf();//fout buffer
				cout.rdbuf(fb);
				bool foutput=true;
				stringstream ss(command);
				Select(ss,foutput);
				fout.flush();
				fout.close();
				cout.rdbuf(ocb);
			}
		}
//		printTempDatabaseOverall(tmp_database); // 全局输出调试语句 
	
}

void Count(string* word)
{
	cout<<"Count!!"<<endl;
	if(word[2]=="*")
	{
		Database* tmp_database=DB.current_db;
		Table tmp_table=tmp_database->table_list[word[4]];
		cout<<"COUNT(*)"<<endl;
		cout<<tmp_table.row_map.size()<<endl;
	/*	for(auto i:tmp_table.row_map) 
		{
			i.second.outputrow(&tmp_table);
		}*/
	}
	else
	{
		Database* tmp_database=DB.current_db;
		Table tmp_table=tmp_database->table_list[word[4]];
		cout<<"COUNT("<<word[2]<<")"<<endl;
		int outnum=0;
		for(auto i:tmp_table.row_map) 
		{
			i.second.outputrow(&tmp_table);
		}
		cout<<outnum<<endl;
	}
}

