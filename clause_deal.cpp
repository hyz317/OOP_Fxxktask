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
//	if(!cin.getline(cmd,1000,';'))break;//��ָ����ֹ��˵��һ������������break
		string scmd=cmd;//�����ݣ�֮��Ҫ��strtok��
		string WhereString;
	//rr��clause tree���Ե������ӹ��ܣ����������Ǹ�select����� 
	/*	Wherenode *rootnode = nullptr;
		if(scmd.find("WHERE") != -1) {
			WhereString = scmd.substr(scmd.find("WHERE") + 6);
			rootnode = new Wherenode;
			rootnode->prev = nullptr;
		}*/ 
		cout<<__FUNCTION__<<__LINE__<<endl; 
		char *pdeal;//���ڴ��������ָ�� 
		string word[100];//��ÿ�������ɵ��� 
		int how_many_word=0;//word�����Ԫ�ظ���
		pdeal = strtok(cmd,"' ,()\n");//�ÿո�Ȱ�ָ��ֳ�һ��һ���ġ����ʡ����������źͻ��ж��������ˣ� 
		while(pdeal)
		{
			word[how_many_word++]=pdeal;
			pdeal=strtok(NULL,"'\n ,()"); //֮ǰ���ַ������ƻ��ˣ����ҵڶ���ʹ�ÿ�ͷ��ָ�����NULL 
		}
		if(word[1]=="COUNT")Count(word); //��û���ִ�Сд 
		//���Ѿ��зֳɡ����ʡ��������deal_ϵ�к����н��д��� 
	//	deal_database(word,tmp_database,database);//�����NULL����ȥ��ֻ��ͨ������ָ��ſ��Եõ���ָ�� 
	//	deal_datalist(word,tmp_database,how_many_word);
	//	deal_data(word,tmp_database,how_many_word,WhereString,rootnode);
		if(word[3]=="OUTFILE") {
			string OutFileName=word[4];
			fstream check;
			check.open(OutFileName,ios::in);
			if(check){
				cout<<"�ļ��Ѵ��ڣ���һ�����ְɣ�"<<endl;
			}
			else{
				check.close();
				cout<<"���"<<endl;
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
//		printTempDatabaseOverall(tmp_database); // ȫ������������ 
	
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

