#include "clause_deal.h"
#include "Database.h"
#include "DatabaseMap.h"
#include "Command.h"
#include "Row.h"
#include <cstring>
#include <fstream>
#include <sstream>
#include <set>
#include "Wherenode.h"
extern DatabaseMap DB;
using namespace std;
//void Select(std::stringstream& ss,bool foutput=false);
Wherenode *rootnode = nullptr;

void clause_deal(char* cmd,string command)
{
	cout<<"deal"<<endl;
//	char cmd[1020];
//	if(!cin.getline(cmd,1000,';'))break;//��ָ����ֹ��˵��һ������������break
		string scmd=cmd;//�����ݣ�֮��Ҫ��strtok��
		string WhereString;
	//rr��clause tree���Ե������ӹ��ܣ����������Ǹ�select����� 
		
		if(scmd.find("WHERE") != -1) {
			WhereString = scmd.substr(scmd.find("WHERE") + 6);
			rootnode = new Wherenode;
			
		//	rootnode->prev = nullptr;
		} 
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
	
		//���뵼�� 
		if(Find(word,"OUTFILE",how_many_word)!=-1) {//��֪��SELECT�����*�ǲ���ֻ��һ���� 
			Output(word,command);
		}
		if(word[0]=="LOAD"){//��ʱ���»��費��Ҫ��һ������ 
			Input(word,how_many_word);
		}
		
		MathFunction mathfuntion(word,how_many_word);//����ģʽ 
		if(mathfuntion.Deal())return;
		//wtr�ĵ�һ�����У�select + count + groupby 
		if(Find(word,"SELECT",how_many_word)!=-1&&Find(word,"COUNT",how_many_word)!=-1&&
		Find(word,"GROUP",how_many_word)!=-1&&Find(word,"ORDER",how_many_word)==-1)Group_by(word,how_many_word); 
		
		//�¼ӵĵڶ������У�select + count + groupby + orderby 
		if(Find(word,"SELECT",how_many_word)!=-1&&Find(word,"COUNT",how_many_word)!=-1&&
		Find(word,"GROUP",how_many_word)!=-1&&Find(word,"ORDER",how_many_word)!=-1)Group_by(word,how_many_word,word[Find(word,"ORDER",how_many_word)+2]); 
		
		//�¼ӵĵ��������У�select + count + orderby 
		if(Find(word,"SELECT",how_many_word)!=-1&&Find(word,"COUNT",how_many_word)==-1&&
		Find(word,"GROUP",how_many_word)==-1&&Find(word,"ORDER",how_many_word)!=-1) NewSelect(word,how_many_word,WhereString,word[Find(word,"ORDER",how_many_word)+2]); 
//		printTempDatabaseOverall(tmp_database); // ȫ������������ 
	
		if(Find(word,"UNION",how_many_word)!=-1){
			Union(word,how_many_word,scmd);
		}
}

bool OrderByCompare2(const string &a1, const string &a2, string type) // һ���ж����� value ��С�ĺ��� 
{
	if(a1 == "NULL")
		return a1 < a2;
	if(a2 == "NULL")
		return a2 < a1;
	if(type == "int")
		return stoi(a1) < stoi(a2);
	if(type == "double")
		return stod(a1) < stod(a2);
	if(type == "char")
		return a1 < a2; 
	if(type == "date")
		return a1 < a2;
	if(type == "time")
		return a1 < a2; 
} 

int Find(string *word,string keyword,int how_many_word) // �� string ����������ȷ���±� 
{
	for(int i=0;i<how_many_word;i++)
	{
		if(word[i]==keyword)return i;
	}
	return -1;
}

void Output(string* word,string command){
	string OutFileName=word[4];//�������ѱ�ɾ��
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

void Input(string* word,int how_many_word){
	string InFileName=word[3];
	fstream check;
	check.open(InFileName,ios::in);
	if(!check){
		cout<<"�ļ������ڣ�"<<endl; 
	}
	else{
		int pos=Find(word,"TABLE",how_many_word);
		string tableName=word[pos+1];
		cout<<"table: "<<tableName<<endl;
		vector<string> attrs;
		for(int i=pos+2;i<how_many_word;i++){
			attrs.push_back(word[i]);
		}
		Table& current_table=DB.current_db->table_list[tableName];//��������ã���Ϊ�������Ƴ����ľͲ���ԭ���Ǹ����ˣ�����data_list����Ҽ�ʹ�¼ӽ�ȥ��Ҳ�������� 
		//cout<<"key: "<<current_table.getkey()<<endl;
		bool ok=true;
		while(!check.eof()){
			string temp;
			getline(check,temp);//��ȡÿһ�� 
			stringstream input(temp);
			string buffer;
			vector<string> values;
			for(int i=0;i<attrs.size();i++){
				input>>buffer;
				if(buffer.size()==0){//��Ϊ�������һ���յģ���ʱ���������insert�ͻ�����⣬������Ҫ����һ�� 
					ok=false;
					break;
				}
				values.push_back(buffer);
			}
			if(ok){
				current_table.insert(attrs,values);//���õ�һ�׶ε�insert���� 
			}
		}
		check.close();
	}
}

//SELECT stu_name, COUNT(*) from oop_info GROUP BY stu_name
//select ��ѡ�����
//count ��ѡ�����
//group by�ǹ���Щ�� 
void Group_by(string *word,int how_many_word, string order_by_attr) // order_by_attr �ǿ��ܴ��ڵ���Ҫ orderby ���� 
{
	cout<<"groupby"<<endl;
	//���count group��index 
	int count_index=Find(word,"COUNT",how_many_word);
	int group_index=Find(word,"GROUP",how_many_word);
	string count_col=word[count_index+1]; 

	vector<string> basic; // ��¼��Ҫ select ������ 
	for(int i=group_index+2;i<how_many_word && word[i]!="ORDER";i++)
	{
		cout<<"want to push "<<word[i]<<endl;
		basic.push_back(word[i]);
	}
	map<vector<string>,int> group; // select���е�ÿһ��������ȫһ��ʱ�Żᱻ�ǵ�һ��key�� 
	Database* tmp_database=DB.current_db;
	cout<<word[count_index+3]<<endl;
//���� 
	if(tmp_database!=NULL)
	for(auto i:tmp_database->table_list)
	{
		cout<<i.first<<endl;
	}
	Table tmp_table=tmp_database->table_list[word[count_index+3]];//�ҵ�Ŀǰʹ�õ�table

	for(auto i:tmp_table.row_map) // ������ǰ table �е������� 
	{
		cout<<"!"<<endl;
		vector<string> tmp_row; // ÿ�γ��һ���У����������� push �� tmp_row �� 
		bool ok=1; 
		for(int j=0;j<basic.size();j++)
		{
			cout<<"want to back"<<i.second.data[basic[j]]<<endl;
			if(basic[j]==count_col) //�������count����
			{
				if(i.second.data[basic[j]]=="NULL")ok=0; // ������ֵ 
			} 
			tmp_row.push_back(i.second.data[basic[j]]);
		}
		//++����Ҫ���� 
		if(!ok)continue;
		if(group.count(tmp_row)==0)group[tmp_row]=1;
		else group[tmp_row]++;
	}
	int index[10];//��¼��Ӧ��ϵ Ҫ����ĵ�i���ж�Ӧvector�еĵڼ���
	int num_of_attr = -1; // ��¼��Ҫ orderby ���ж�Ӧ�����֣� 
	for(int j=1;j<count_index;j++)
	{
		for(int i=0;i<basic.size();i++)
		{
			if(basic[i]==word[j])
			{
				index[j-1]=i;
				if(basic[i] == order_by_attr) num_of_attr = j-1;	
				break;
			}	
		}	
	}
	cout<<"index"<<index[0]<<endl; 	
	for(int j=1;j<count_index;j++)
	{
		cout<<word[j]<<'\t';
	}
	cout<<"COUNT\n";
	// \t������֪����û�й�ϵ 
	//�������������п����� orderby ��������� 
	if(order_by_attr == "") // ���û�� orderby��������� 
	    for(auto i:group)
	    {
			for(int j=0;j<count_index-1;j++)
	    	{
	    		cout<<i.first[index[j]]<<"\t";
			}
			cout<<i.second; 
			cout<<endl;
		}
	else if(order_by_attr != "COUNT") { // ����� orderby �Ҳ��� order Count(*)��һ�� 
		string tmp_type = GetOrderbyType(word[count_index+3], order_by_attr);

		while(!group.empty()) {
			vector<string> nowvector = (*group.begin()).first;
			for (auto i: group) {
				if(OrderByCompare2(i.first[index[num_of_attr]], nowvector[index[num_of_attr]], tmp_type))
					nowvector = i.first;
			} // �ҵ�orderby����С���� 
			for(int j=0;j<count_index-1;j++)
	    	{
	    		cout<<nowvector[index[j]]<<'\t';
			} // ������е���Ϣ 
			cout<<group[nowvector];//�˴���û���ƣ���ʵӦ���ж�count���� 
			cout<<endl;
			group.erase(nowvector);//���������Ϣ�󽫸��д�map���Ƴ�������Ѱ����С���У�ֱ��mapΪ�� 
		}
	}
	else { // ����� orderby ���� order Count(*)��һ�� 
		while(!group.empty()) {
			int minm = (1<<30);
			vector<string> nowvector = (*group.begin()).first;
			for (auto i: group) {
				if(i.second < minm) {
					minm = i.second;
					nowvector = i.first;
				}
			} // �ҵ�orderby����С���� 
			for(int j=0;j<count_index-1;j++)
	    	{
	    		cout<<nowvector[index[j]]<<'\t';
			} // ������е���Ϣ 
			cout<<group[nowvector];//�˴���û���ƣ���ʵӦ���ж�count���� 
			cout<<endl;
			group.erase(nowvector);
		}
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

string GetOrderbyType(string table_name, string order_by_attr) // ���ڷ�����Ҫ orderby ���е����� int char double 
{
	for(auto i:DB.current_db->table_list[table_name].attr_list){
		if(i.name == order_by_attr)
			return i.type;
	}
}

bool OrderByCompare(string table_name, string order_by_attr, const Data& a1, const Data& a2, string type) //���ڱȽ�����������Ӧorderby�е�ֵ�Ĵ�С 
{
	if(DB.current_db->table_list[table_name].row_map[a1].data[order_by_attr] == "NULL")
		return a1 < a2;
	else if(DB.current_db->table_list[table_name].row_map[a2].data[order_by_attr] == "NULL")
		return a2 < a1;
	else if(type == "int")
		return stoi(DB.current_db->table_list[table_name].row_map[a1].data[order_by_attr]) < stoi(DB.current_db->table_list[table_name].row_map[a2].data[order_by_attr]);
	else if(type == "double")
		return stod(DB.current_db->table_list[table_name].row_map[a1].data[order_by_attr]) < stod(DB.current_db->table_list[table_name].row_map[a2].data[order_by_attr]);
	else if(type == "char" || type == "date" || type == "time")
		return DB.current_db->table_list[table_name].row_map[a1].data[order_by_attr] < DB.current_db->table_list[table_name].row_map[a2].data[order_by_attr];
}

void NewSelect(string *word, int how_many_word, string wherestring, string order_by_attr) { // ���ڴ��� select + orderby ����� 
	cout<<"NewSelect\n";
	cout<<"wherestring "<<wherestring<<endl<<"order_by_attr "<<order_by_attr<<endl;
	int count_index = Find(word, "COUNT", how_many_word);
	int from_index = Find(word, "FROM", how_many_word);
	int where_index = Find(word, "WHERE", how_many_word);
	
	std::string table_name;	
	std::vector<std::string> attr_name;
	
	for(int i = 1; word[i] != "FROM"; i++)
		attr_name.push_back(word[i]); // ������ select ���о����� attr_name ��� vector ���汸�� 
	if (attr_name[0] == "*") {
			
		table_name = word[from_index + 1]; // ��ȡ��ǰ table 
		if (where_index == -1) {
			//table_name = table_name.substr(0, table_name.length() - 1);
			std::set<Data> key_of_rows = DB.GetAllKeys(table_name); // ��ȡ������ 
			if(key_of_rows.begin() != key_of_rows.end()) // �ǿվ������ͷ 
				DB.OutputAttr(table_name);
			if(order_by_attr == "") // ���û�� orderby����ôֱ����� 
				for (auto i = key_of_rows.begin(); i != key_of_rows.end(); i++) {
					DB.OutputRow(table_name, (*i).value);
				}
			else { // ����� orderby����ô�ҵ���С�У�������������д�set��ɾ�����ظ��ò���ֱ��setΪ�� 
				string tmp_type = GetOrderbyType(table_name, order_by_attr);
				while(!key_of_rows.empty()) {
					Data nowdata = *key_of_rows.begin();
					for (auto i: key_of_rows) {
						if(OrderByCompare(table_name, order_by_attr, i, nowdata, tmp_type))
							nowdata = i;
					}
					DB.OutputRow(table_name, nowdata.value);
					key_of_rows.erase(nowdata);
				}
			}
			return;
		}
		else {
			StringSplit(wherestring, rootnode, &DB.current_db->table_list[table_name]);
			//std::set<Data> key_of_rows = where_clause(table_name, wherestring);
			std::set<Data> key_of_rows = getWhereKeys(rootnode, &DB.current_db->table_list[table_name]);
			if(key_of_rows.begin() != key_of_rows.end())
				DB.OutputAttr(table_name);
			if(order_by_attr == "") 
				for (auto i = key_of_rows.begin(); i != key_of_rows.end(); i++) {
					DB.OutputRow(table_name, (*i).value);
				}
			else {
				string tmp_type = GetOrderbyType(table_name, order_by_attr);
				while(!key_of_rows.empty()) {
					Data nowdata = *key_of_rows.begin();
					for (auto i: key_of_rows) {
						if(OrderByCompare(table_name, order_by_attr, i, nowdata, tmp_type))
							nowdata = i;
					}
					DB.OutputRow(table_name, nowdata.value);
					key_of_rows.erase(nowdata);
				}
			}
			return;
		}

	}
	else {
		
		table_name = word[from_index + 1];
		
		if (where_index == -1) {
			//table_name = table_name.substr(0, table_name.length() - 1);
			for (auto i = attr_name.begin(); i < attr_name.end() - 1; i++) {
				std::cout << (*i) << "\t";
			}
			std::cout << *(attr_name.end()-1) << "\n" ;
			std::set<Data> key_of_rows = DB.GetAllKeys(table_name);
			
			if(order_by_attr == "") {
				for (auto i = key_of_rows.begin(); i != key_of_rows.end(); i++) {
					for (int j = 0; j < attr_name.size() - 1; j++) {
						std::string value = DB.GetValue(table_name, attr_name[j], (*i).value);
						std::string type = DB.GetType(table_name, attr_name[j]);
						OutputData(value, type);
						std::cout << "\t";
					}
					std::string value = DB.GetValue(table_name, attr_name[attr_name.size()-1], (*i).value);
					std::string type = DB.GetType(table_name, attr_name[attr_name.size() - 1]);
					OutputData(value, type);
					std::cout << "\n";
				}
			}
			else {
				string tmp_type = GetOrderbyType(table_name, order_by_attr);
				while(!key_of_rows.empty()) {
					Data nowdata = *key_of_rows.begin();
					for (auto i: key_of_rows) {
						if(OrderByCompare(table_name, order_by_attr, i, nowdata, tmp_type))
							nowdata = i;
					}
					for (int j = 0; j < attr_name.size() - 1; j++) {
						std::string value = DB.GetValue(table_name, attr_name[j], nowdata.value);
						std::string type = DB.GetType(table_name, attr_name[j]);
						OutputData(value, type);
						std::cout << "\t";
					}
					std::string value = DB.GetValue(table_name, attr_name[attr_name.size()-1], nowdata.value);
					std::string type = DB.GetType(table_name, attr_name[attr_name.size() - 1]);
					OutputData(value, type);
					std::cout << "\n";

					key_of_rows.erase(nowdata);
				}
			}
			
			return;
		}
		
		else {
			for (auto i = attr_name.begin(); i < attr_name.end() - 1; i++) {
				std::cout << (*i) << "\t";
			}
			std::cout << *(attr_name.end()-1) << "\n" ;
			StringSplit(wherestring, rootnode, &DB.current_db->table_list[table_name]);
			//std::set<Data> key_of_rows = where_clause(table_name, wherestring);
			std::set<Data> key_of_rows = getWhereKeys(rootnode, &DB.current_db->table_list[table_name]);
			
			if(order_by_attr == "") {
				for (auto i = key_of_rows.begin(); i != key_of_rows.end(); i++) {
					for (int j = 0; j < attr_name.size() - 1; j++) {
						std::string value = DB.GetValue(table_name, attr_name[j], (*i).value);
						std::string type = DB.GetType(table_name, attr_name[j]);
						OutputData(value, type);
						std::cout << "\t";
					}
					std::string value = DB.GetValue(table_name, attr_name[attr_name.size()-1], (*i).value);
					std::string type = DB.GetType(table_name, attr_name[attr_name.size() - 1]);
					OutputData(value, type);
					std::cout << "\n";
				}
			}
			else {
				string tmp_type = GetOrderbyType(table_name, order_by_attr);
				while(!key_of_rows.empty()) {
					Data nowdata = *key_of_rows.begin();
					for (auto i: key_of_rows) {
						if(OrderByCompare(table_name, order_by_attr, i, nowdata, tmp_type))
							nowdata = i;
					}
					for (int j = 0; j < attr_name.size() - 1; j++) {
						std::string value = DB.GetValue(table_name, attr_name[j], nowdata.value);
						std::string type = DB.GetType(table_name, attr_name[j]);
						OutputData(value, type);
						std::cout << "\t";
					}
					std::string value = DB.GetValue(table_name, attr_name[attr_name.size()-1], nowdata.value);
					std::string type = DB.GetType(table_name, attr_name[attr_name.size() - 1]);
					OutputData(value, type);
					std::cout << "\n";

					key_of_rows.erase(nowdata);
				}
			}
		}
	}
}

void pickitem(set<vector<string>>& selected,Table table,vector<string> attrName,string wherestring,int orderbynum){
	int count=0;
	vector<string> temp;
	if(wherestring=="NULL"){
		for(auto i=table.row_map.begin();i!=table.row_map.end();i++){
			temp.clear();
			for(auto j:attrName){
				cout<<"temp_pushback "<<i->second.data[j]<<" now temp size:["<<temp.size()<<"] attrname:"<<j<<endl;
				/*if(i->second.data[j] != "")*/ temp.push_back(i->second.data[j]);
			}
			selected.insert(temp);
		}
	}
	else{
		StringSplit(wherestring, rootnode, &table);
		set<Data> key_of_rows=getWhereKeys(rootnode,&table);
		for(auto i:key_of_rows){
			temp.clear();
			for(auto j:attrName){
				temp.push_back(table.row_map[i].data[j]);
			}
			selected.insert(temp);
		}
	}
}
void multipickitem(multiset<vector<string>>& multiselected,Table table,vector<string> attrName,string wherestring,int orderbynum){
	int count=0;
	vector<string> temp;
	if(wherestring=="NULL"){
		for(auto i=table.row_map.begin();i!=table.row_map.end();i++){
			temp.clear();
			for(auto j:attrName){
				temp.push_back(i->second.data[j]);
			}
			multiselected.insert(temp);
		}
	}
	else{
		StringSplit(wherestring, rootnode, &table);
		set<Data> key_of_rows=getWhereKeys(rootnode,&table);
		for(auto i:key_of_rows){
			temp.clear();
			for(auto j:attrName){
				temp.push_back(table.row_map[i].data[j]);
			}
			multiselected.insert(temp);
		}
	}
}
bool UnionCompare(vector<string> v1,vector<string>v2,string orderbytype,int orderbynum){
	if(orderbytype=="int"){
		return stoi(v1[orderbynum])<stoi(v2[orderbynum]);
	}
	else if(orderbytype=="double"){
		return stod(v1[orderbynum])<stod(v2[orderbynum]);
	}
	else if(orderbytype=="char" || orderbytype=="date" || orderbytype=="time"){
		return v1[orderbynum]<v2[orderbynum];
	}
	else{
		cout<<"Hey what are you doing???"<<endl;
	}
}
void Union(string* word,int how_many_word,string scmd){
	bool multi=false;//�ж��Ƿ�Ϊ"UNION ALL" 
	set<vector<string>> selected;
	multiset<vector<string>> multiselected;
	if(Find(word,"ALL",how_many_word)!=-1){
		multi=true;
	}
	string orderbyattrName;//�������������һ�����������ϵ�˵�����ְ���һ����ı�ͷ�����ߣ�
	int orderbynum;//������������ĵڼ�������Ϊ���ܲ�ͬ���ı�ͷ���ֲ�һ���� 
	string orderbytype;//��ʲô���� 
	
	string* p;//ָ��ĳһ��word��ָ�룬ÿ�δ��ϴ�û�鵽�ĵط���ʼ 
	vector<string> attrName;//�����ж������Ҫ�� 
	string tableName;
	Table u_table;
	int pos;//"FROM"��λ��
	int wherepos;
	string wherestr="NULL";//֮��ÿ�ζ�Ҫ��ʼ��
	
	p=word;
	pos=Find(p,"FROM",how_many_word);//�����UNION�Ļ������һ��UNION��������ű����Ϣ����Ū
	orderbyattrName=word[Find(p,"ORDER",how_many_word)+2];
	cout<<orderbyattrName<<endl;
	for(int i=Find(p,"SELECT",how_many_word)+1;i<pos;i++){
		attrName.push_back(word[i]);
		cout<<"attrname.push_back "<<word[i]<<endl;
		if(word[i]==orderbyattrName){
			orderbynum=i-Find(p,"SELECT",how_many_word)-1;
		}
	}
	tableName=p[pos+1];
	cout<<tableName<<endl;
	orderbytype=GetOrderbyType(tableName,orderbyattrName);
	cout<<orderbytype<<endl;
	u_table=DB.current_db->table_list[tableName];//��ΪҪ������������Ԫ�����Կ�������һ���µ�û�й�ϵ
	if(word[pos+2]=="WHERE"){
		scmd=scmd.substr(scmd.find("WHERE")+6);
		wherestr=scmd.substr(0,scmd.find("UNION")-1);//�ڶ��������ǳ��ȣ������Ǵ�0��ʼ��������ν 
	}
	
	if(!multi){
		pickitem(selected,u_table,attrName,wherestr,orderbynum);
	}
	else{
		multipickitem(multiselected,u_table,attrName,wherestr,orderbynum);
	}
	p=p+pos+2;
	how_many_word-=(pos+2);
	pos=Find(p,"FROM",how_many_word);
	wherestr="NULL";
	while(pos!=-1){
		for(int i=Find(p,"SELECT",how_many_word)+1;i<pos;i++){
			attrName.push_back(word[i]);
			cout<<"��2��?? attr.name_push_back "<<word[i]<<endl;
		}
		tableName=p[pos+1];//pos�Ƕ�p�����λ��ѽ 
		u_table=DB.current_db->table_list[tableName];
		if(word[pos+2]=="WHERE"){
			scmd=scmd.substr(scmd.find("WHERE")+6);
			wherestr=scmd.substr(0,scmd.find("UNION")-1);//�ڶ��������ǳ��ȣ������Ǵ�0��ʼ��������ν 
		}
		//cout<<tableName<<endl;
		if(!multi){
			pickitem(selected,u_table,attrName,wherestr,orderbynum);
		}
		else{
			multipickitem(multiselected,u_table,attrName,wherestr,orderbynum);
		}
		p=p+pos+2;
		how_many_word-=(pos+2);
		pos=Find(p,"FROM",how_many_word); 
		wherestr="NULL";
	}
	if(!multi){
		cout<<"selected"<<' '<<selected.size()<<endl;
		vector<string> tmp=*selected.begin();//����ظ�Ԫ�أ������Ϊʲô�� 
		for(auto i:selected){
			cout<<"size ["<<i.size()<<"] ";
			for(auto j:i){
				cout<<j<<"*";
			}
			cout<<endl;
			cout<<(i==tmp)<<endl;
			tmp=i;
		}
		cout<<"selected"<<endl;
		int count=0;
		vector<string> minimum=*selected.begin();
		while(!selected.empty()){
			for(auto i:selected){
				if(UnionCompare(i,minimum,orderbytype,orderbynum)){
					minimum=i;
				}
			}
			for(auto i:minimum){
				cout<<i<<'\t';
			}
			cout<<endl;
			selected.erase(minimum);//�ǵ������
		}
	}
	else{
		vector<string> minimum=*multiselected.begin();
		while(!multiselected.empty()){
			for(auto i:selected){
				if(UnionCompare(i,minimum,orderbytype,orderbynum)){
					minimum=i;
				}
			}
			for(auto i:minimum){
				cout<<i<<'\t';
			}
			cout<<endl;
			multiselected.erase(minimum);//�ǵ������
		}
	}
}
