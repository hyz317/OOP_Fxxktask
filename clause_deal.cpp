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
void Group_by(string*,int, string s = "");
int Find(string*,string,int);
void NewSelect(string*, int, string, string order_by_attr = "");
string GetOrderbyType(string table_name, string order_by_attr);
Wherenode *rootnode = nullptr;
void Output(string* word,string command);
void Input(string* word,int how_many_word);
void Union(string* word,int how_many_word);

void clause_deal(char* cmd,string command)
{
	cout<<"deal"<<endl;
//	char cmd[1020];
//	if(!cin.getline(cmd,1000,';'))break;//若指令终止，说明一条语句结束，则break
		string scmd=cmd;//做备份（之后要用strtok）
		string WhereString;
	//rr的clause tree可以当作附加功能，不用它的那个select里面的 
		
		if(scmd.find("WHERE") != -1) {
			WhereString = scmd.substr(scmd.find("WHERE") + 6);
			rootnode = new Wherenode;
			
		//	rootnode->prev = nullptr;
		} 
	//	cout<<__FUNCTION__<<__LINE__<<endl; 
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
	
		//导入导出 
		if(Find(word,"OUTFILE",how_many_word)!=-1) {//不知道SELECT后面的*是不是只有一个词 
			Output(word,command);
		}
		if(word[0]=="LOAD"){//到时候看下还需不需要进一步特判 
			Input(word,how_many_word);
		}
		
		MathFunction mathfuntion(word,how_many_word);//工厂模式 
		if(mathfuntion.Deal())return;
		
		//wtr的第一个特判，select + count + groupby 
		if(Find(word,"SELECT",how_many_word)!=-1&&Find(word,"COUNT",how_many_word)!=-1&&
		Find(word,"GROUP",how_many_word)!=-1&&Find(word,"ORDER",how_many_word)==-1)Group_by(word,how_many_word); 
		
		//新加的第二个特判，select + count + groupby + orderby 
		if(Find(word,"SELECT",how_many_word)!=-1&&Find(word,"COUNT",how_many_word)!=-1&&
		Find(word,"GROUP",how_many_word)!=-1&&Find(word,"ORDER",how_many_word)!=-1)Group_by(word,how_many_word,word[Find(word,"ORDER",how_many_word)+2]); 
		
		//新加的第三个特判，select + count + orderby 
		if(Find(word,"SELECT",how_many_word)!=-1&&Find(word,"COUNT",how_many_word)==-1&&
		Find(word,"GROUP",how_many_word)==-1&&Find(word,"ORDER",how_many_word)!=-1) NewSelect(word,how_many_word,WhereString,word[Find(word,"ORDER",how_many_word)+2]); 
//		printTempDatabaseOverall(tmp_database); // 全局输出调试语句 
	
		if(Find(word,"UNION",how_many_word)!=-1){
			Union(word,how_many_word);
		}
}

bool OrderByCompare2(const string &a1, const string &a2, string type) // 一个判断两个 value 大小的函数 
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
} 

int Find(string *word,string keyword,int how_many_word) // 从 string 数组中找正确的下标 
{
	for(int i=0;i<how_many_word;i++)
	{
		if(word[i]==keyword)return i;
	}
	return -1;
}

void Output(string* word,string command){
	string OutFileName=word[4];//单引号已被删除
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

void Input(string* word,int how_many_word){
	string InFileName=word[3];
	fstream check;
	check.open(InFileName,ios::in);
	if(!check){
		cout<<"文件不存在！"<<endl; 
	}
	else{
		int pos=Find(word,"TABLE",how_many_word);
		string tableName=word[pos+1];
		cout<<"table: "<<tableName<<endl;
		vector<string> attrs;
		for(int i=pos+2;i<how_many_word;i++){
			attrs.push_back(word[i]);
		}
		Table& current_table=DB.current_db->table_list[tableName];//必须得引用，因为拷贝复制出来的就不是原来那个表了！不在data_list里，而且即使新加进去了也是两个表 
		//cout<<"key: "<<current_table.getkey()<<endl;
		bool ok=true;
		while(!check.eof()){
			string temp;
			getline(check,temp);//读取每一行 
			stringstream input(temp);
			string buffer;
			vector<string> values;
			for(int i=0;i<attrs.size();i++){
				input>>buffer;
				if(buffer.size()==0){//因为最后会读入一个空的，这时如果还把它insert就会出问题，所以需要特判一下 
					ok=false;
					break;
				}
				values.push_back(buffer);
			}
			if(ok){
				current_table.insert(attrs,values);//利用第一阶段的insert函数 
			}
		}
		check.close();
	}
}

//SELECT stu_name, COUNT(*) from oop_info GROUP BY stu_name
//select 是选择输出
//count 是选择计数
//group by是归哪些列 
void Group_by(string *word,int how_many_word, string order_by_attr) // order_by_attr 是可能存在的需要 orderby 的列 
{
	cout<<"groupby"<<endl;
	//获得count group的index 
	int count_index=Find(word,"COUNT",how_many_word);
	int group_index=Find(word,"GROUP",how_many_word);
	string count_col=word[count_index+1]; 

	vector<string> basic; // 记录需要 select 的列名 
	for(int i=group_index+2;i<how_many_word && word[i]!="ORDER";i++)
	{
		cout<<"want to push "<<word[i]<<endl;
		basic.push_back(word[i]);
	}
	map<vector<string>,int> group; // select列中的每一行数据完全一样时才会被记到一个key中 
	Database* tmp_database=DB.current_db;
	cout<<word[count_index+3]<<endl;
//？？ 
	if(tmp_database!=NULL)
	for(auto i:tmp_database->table_list)
	{
		cout<<i.first<<endl;
	}
	Table tmp_table=tmp_database->table_list[word[count_index+3]];//找到目前使用的table

	for(auto i:tmp_table.row_map) // 遍历当前 table 中的所有行 
	{
		cout<<"!"<<endl;
		vector<string> tmp_row; // 每次抽出一个行，把所有数据 push 进 tmp_row 中 
		bool ok=1; 
		for(int j=0;j<basic.size();j++)
		{
			cout<<"want to back"<<i.second.data[basic[j]]<<endl;
			if(basic[j]==count_col) //如果发现count的列
			{
				if(i.second.data[basic[j]]=="NULL")ok=0; // 跳过空值 
			} 
			tmp_row.push_back(i.second.data[basic[j]]);
		}
		//++条件要改下 
		if(!ok)continue;
		if(group.count(tmp_row)==0)group[tmp_row]=1;
		else group[tmp_row]++;
	}
	int index[10];//记录对应关系 要输出的第i个列对应vector中的第几个
	int num_of_attr = -1; // 记录需要 orderby 的列对应的数字！ 
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
	// \t数量不知道有没有关系 
	//以下是新增的有可能有 orderby 的输出部分 
	if(order_by_attr == "") // 如果没有 orderby，正常输出 
	    for(auto i:group)
	    {
			for(int j=0;j<count_index-1;j++)
	    	{
	    		cout<<i.first[index[j]]<<"\t";
			}
			cout<<i.second; 
			cout<<endl;
		}
	else if(order_by_attr != "COUNT") { // 如果有 orderby 且不是 order Count(*)这一列 
		string tmp_type = GetOrderbyType(word[count_index+3], order_by_attr);

		while(!group.empty()) {
			vector<string> nowvector = (*group.begin()).first;
			for (auto i: group) {
				if(OrderByCompare2(i.first[index[num_of_attr]], nowvector[index[num_of_attr]], tmp_type))
					nowvector = i.first;
			} // 找到orderby列最小的行 
			for(int j=0;j<count_index-1;j++)
	    	{
	    		cout<<nowvector[index[j]]<<'\t';
			} // 输出这行的信息 
			cout<<group[nowvector];//此处还没完善，其实应该判断count有无 
			cout<<endl;
			group.erase(nowvector);//输出该行信息后将该行从map中移出，重新寻找最小的行，直到map为空 
		}
	}
	else { // 如果有 orderby 且是 order Count(*)这一列 
		while(!group.empty()) {
			int minm = (1<<30);
			vector<string> nowvector = (*group.begin()).first;
			for (auto i: group) {
				if(i.second < minm) {
					minm = i.second;
					nowvector = i.first;
				}
			} // 找到orderby列最小的行 
			for(int j=0;j<count_index-1;j++)
	    	{
	    		cout<<nowvector[index[j]]<<'\t';
			} // 输出这行的信息 
			cout<<group[nowvector];//此处还没完善，其实应该判断count有无 
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

string GetOrderbyType(string table_name, string order_by_attr) // 用于返回需要 orderby 的列的类型 int char double 
{
	for(auto i:DB.current_db->table_list[table_name].attr_list){
		if(i.name == order_by_attr)
			return i.type;
	}
}

bool OrderByCompare(string table_name, string order_by_attr, const Data& a1, const Data& a2, string type) //用于比较两个行所对应orderby列的值的大小 
{
	if(DB.current_db->table_list[table_name].row_map[a1].data[order_by_attr] == "NULL")
		return a1 < a2;
	else if(DB.current_db->table_list[table_name].row_map[a2].data[order_by_attr] == "NULL")
		return a2 < a1;
	else if(type == "int")
		return stoi(DB.current_db->table_list[table_name].row_map[a1].data[order_by_attr]) < stoi(DB.current_db->table_list[table_name].row_map[a2].data[order_by_attr]);
	else if(type == "double")
		return stod(DB.current_db->table_list[table_name].row_map[a1].data[order_by_attr]) < stod(DB.current_db->table_list[table_name].row_map[a2].data[order_by_attr]);
	else if(type == "char")
		return DB.current_db->table_list[table_name].row_map[a1].data[order_by_attr] < DB.current_db->table_list[table_name].row_map[a2].data[order_by_attr];
}

void NewSelect(string *word, int how_many_word, string wherestring, string order_by_attr) { // 用于处理 select + orderby 的情况 
	cout<<"NewSelect\n";
	cout<<"wherestring "<<wherestring<<endl<<"order_by_attr "<<order_by_attr<<endl;
	int count_index = Find(word, "COUNT", how_many_word);
	int from_index = Find(word, "FROM", how_many_word);
	int where_index = Find(word, "WHERE", how_many_word);
	
	std::string table_name;	
	std::vector<std::string> attr_name;
	
	for(int i = 1; word[i] != "FROM"; i++)
		attr_name.push_back(word[i]); // 把所有 select 的列均放入 attr_name 这个 vector 里面备用 
	if (attr_name[0] == "*") {
			
		table_name = word[from_index + 1]; // 获取当前 table 
		if (where_index == -1) {
			//table_name = table_name.substr(0, table_name.length() - 1);
			std::set<Data> key_of_rows = DB.GetAllKeys(table_name); // 获取所有行 
			if(key_of_rows.begin() != key_of_rows.end()) // 非空就输出表头 
				DB.OutputAttr(table_name);
			if(order_by_attr == "") // 如果没有 orderby，那么直接输出 
				for (auto i = key_of_rows.begin(); i != key_of_rows.end(); i++) {
					DB.OutputRow(table_name, (*i).value);
				}
			else { // 如果有 orderby，那么找到最小行，输出，并将这行从set中删除，重复该操作直到set为空 
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

void pickitem(set<string>& selected,Table table,string attrName){
	for(auto i=table.row_map.begin();i!=table.row_map.end();i++){
		selected.insert(i->second.data[attrName]);
	}
}
void multipickitem(multiset<string>& multiselected,Table table,string attrName){
	for(auto i=table.row_map.begin();i!=table.row_map.end();i++){
		multiselected.insert(i->second.data[attrName]);
	}
}
void Union(string* word,int how_many_word){
	bool multi=false;//是否为"UNIONALL" 
	set<string> selected;
	multiset<string> multiselected;
	if(Find(word,"ALL",how_many_word)!=-1){
		multi=true;
	}
	string attrName=word[1];
	string tableName;
	Table u_table;
	string* p=word;
	int pos=Find(word,"FROM",how_many_word);//如果用UNION的话，最后一个UNION后面的那张表的信息不好弄
	tableName=p[pos+1];
	cout<<tableName<<endl;
	u_table=DB.current_db->table_list[tableName];//因为要重新输出里面的元素所以拷贝构造一个新的没有关系
	if(!multi){
		pickitem(selected,u_table,attrName);
	}
	else{
		multipickitem(multiselected,u_table,attrName);
	}
	p=p+pos+2;
	cout<<"pos: "<<pos<<endl;
	pos=Find(p,"FROM",how_many_word-pos-2);
	cout<<"pos: "<<pos<<endl;
	while(pos!=-1){
		tableName=p[pos+1];//pos是对p的相对位置呀 
		u_table=DB.current_db->table_list[tableName];
		cout<<tableName<<endl;
		if(!multi){
			pickitem(selected,u_table,attrName);
		}
		else{
			multipickitem(multiselected,u_table,attrName);
		}
		p=p+pos+2;
		pos=Find(p,"FROM",how_many_word-pos-2); 
	}
	if(!multi){
		for(auto i:selected){//我觉得这个也属于“可能无主键输出不确定所以一定会有ORDER BY”的范围吧..?不能的话就用map映射值和顺序 
			cout<<i<<endl;
		}
	}
	else{
		for(auto i:multiselected){
			cout<<i<<endl;
		}
	}
}
