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
void Group_by(string*,int);
int Find(string*,string,int);
void clause_deal(char* cmd,string command)
{
	cout<<"deal"<<endl;
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
	//	cout<<__FUNCTION__<<__LINE__<<endl; 
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
		if(Find(word,"SELECT",how_many_word)!=-1&&Find(word,"COUNT",how_many_word)!=-1&&
		Find(word,"GROUP",how_many_word)!=-1&&Find(word,"SORT",how_many_word)==-1)Group_by(word,how_many_word); 
//		printTempDatabaseOverall(tmp_database); // ȫ������������ 
	
}


int Find(string *word,string keyword,int how_many_word)
{
	for(int i=0;i<how_many_word;i++)
	{
		if(word[i]==keyword)return i;
	}
	return -1;
}
//SELECT stu_name, COUNT(*) from oop_info GROUP BY stu_name
//select ��ѡ�����
//count ��ѡ�����
//group by�ǹ���Щ�� 
void Group_by(string *word,int how_many_word)
{
	cout<<"groupby"<<endl;
	//���count group��index 
	int count_index=Find(word,"COUNT",how_many_word);
	int group_index=Find(word,"GROUP",how_many_word);
	string count_col=word[count_index+1]; 

	vector<string> basic;
	for(int i=group_index+2;i<how_many_word;i++)
	{
		cout<<"want to push "<<word[i]<<endl;
		basic.push_back(word[i]);
	}
	map<vector<string>,int> group;
	Database* tmp_database=DB.current_db;
	cout<<word[count_index+3]<<endl;
//���� 
	if(tmp_database!=NULL)
	for(auto i:tmp_database->table_list)
	{
		cout<<i.first<<endl;
	}
	Table tmp_table=tmp_database->table_list[word[count_index+3]];//�ҵ�Ŀǰʹ�õ�table

	for(auto i:tmp_table.row_map)
	{
		cout<<"!"<<endl;
		vector<string> tmp_row;
		bool ok=1; 
		for(int j=0;j<basic.size();j++)
		{
			cout<<"want to back"<<i.second.data[basic[j]]<<endl;
			if(basic[j]==count_col)//�������count����
			{
				if(i.second.data[basic[j]]=="NULL")ok=0;
			} 
			tmp_row.push_back(i.second.data[basic[j]]);
		}
		//++����Ҫ���� 
		if(!ok)continue;
		if(group.count(tmp_row)==0)group[tmp_row]=1;
		else group[tmp_row]++;
	}
	int index[10];//��¼��Ӧ��ϵ Ҫ����ĵ�i���ж�Ӧvector�еĵڼ���
	for(int j=1;j<count_index;j++)
	{
		for(int i=0;i<basic.size();i++)
		{
			if(basic[i]==word[j])
			{
				index[j-1]=i;	
				break;
			}	
		}	
	}
	cout<<"index"<<index[0]<<endl; 
	cout<<"COUNT ";
	for(int j=1;j<count_index;j++)
	{
		cout<<"\t"<<word[j];
	}cout<<endl;
	// \t������֪����û�й�ϵ 
    for(auto i:group)
    {
		cout<<i.second;//�˴���û���ƣ���ʵӦ���ж�count���� 
		for(int j=0;j<count_index-1;j++)
    	{
    		cout<<'\t'<<i.first[index[j]];
		}cout<<endl;
	}
}
void Count(string* word)
{
	if(DB.current_db!=NULL)cout<<"Count!!"<<endl;
	if(word[2]=="*")
	{
		Database* tmp_database=DB.current_db;
		Table tmp_table=tmp_database->table_list[word[4]];
		cout<<"COUNT(*)"<<endl;
		cout<<tmp_table.row_map.size()<<endl;

	}
	else
	{
		Database* tmp_database=DB.current_db;
		Table tmp_table=tmp_database->table_list[word[4]];
		cout<<"COUNT("<<word[2]<<")"<<endl;
		
		int outnum=0;
		for(auto i:tmp_table.row_map) 
		{
			if(i.second.data[word[2]]!="NULL")outnum++;
		}
		cout<<outnum<<endl;
	}
}


