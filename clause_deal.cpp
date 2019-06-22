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
//	cout<<"deal"<<endl;
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
		Find(word,"GROUP",how_many_word)!=-1&&Find(word,"ORDER",how_many_word)==-1&&Find(word,"UNION",how_many_word)==-1&&Find(word,"JOIN",how_many_word)==-1)
			Group_by(word,how_many_word); 
		
		//新加的第二个特判，select + count + groupby + orderby 
		if(Find(word,"SELECT",how_many_word)!=-1&&Find(word,"COUNT",how_many_word)!=-1&&
		Find(word,"GROUP",how_many_word)!=-1&&Find(word,"ORDER",how_many_word)!=-1&&Find(word,"UNION",how_many_word)==-1&&Find(word,"JOIN",how_many_word)==-1)
			Group_by(word,how_many_word,word[Find(word,"ORDER",how_many_word)+2]); 
		
		//新加的第三个特判，select + count + orderby 
		if(Find(word,"SELECT",how_many_word)!=-1&&Find(word,"COUNT",how_many_word)==-1&&
		Find(word,"GROUP",how_many_word)==-1&&Find(word,"ORDER",how_many_word)!=-1&&Find(word,"UNION",how_many_word)==-1&&Find(word,"JOIN",how_many_word)==-1) 
			NewSelect(word,how_many_word,WhereString,word[Find(word,"ORDER",how_many_word)+2]); 
//		printTempDatabaseOverall(tmp_database); // 全局输出调试语句 
	
		//第四个特判，select + 有机函数 
		if(Find(word,"SELECT",how_many_word)!=-1&&Find(word,"COUNT",how_many_word)==-1&&
		Find(word,"GROUP",how_many_word)==-1&&Find(word,"ORDER",how_many_word)==-1&&Find(word,"UNION",how_many_word)==-1&&Find(word,"JOIN",how_many_word)==-1 &&
		(command.find("LEFT")!=-1||command.find("LCASE")!=-1||command.find("UCASE")!=-1) ) 
			NewSelect(word,how_many_word,WhereString); 
	
		if(Find(word,"UNION",how_many_word)!=-1){
			Union(word,how_many_word,scmd);
		}
		
		if(Find(word,"SELECT",how_many_word)!=-1&&Find(word,"COUNT",how_many_word)==-1&&
		Find(word,"JOIN",how_many_word)!=-1&&Find(word,"ORDER",how_many_word)!=-1) Join(word,how_many_word,WhereString,word[Find(word,"ORDER",how_many_word)+2]);
		
		if(Find(word,"SELECT",how_many_word)!=-1&&Find(word,"COUNT",how_many_word)==-1&&
		Find(word,"JOIN",how_many_word)!=-1&&Find(word,"ORDER",how_many_word)==-1) Join(word,how_many_word,WhereString,"");
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
	if(type == "date")
		return a1 < a2;
	if(type == "time")
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
		//cout<<"输出"<<endl;
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
		//cout<<"table: "<<tableName<<endl;
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
	//cout<<"groupby"<<endl;
	//获得count group的index
	int count_index=Find(word,"COUNT",how_many_word);
	int group_index=Find(word,"GROUP",how_many_word);
	string count_col=word[count_index+1]; 

	vector<string> basic; // 记录需要 select 的列名 
	for(int i=group_index+2;i<how_many_word && word[i]!="ORDER";i++)
	{
		//cout<<"want to push "<<word[i]<<endl;
		basic.push_back(word[i]);
	}
	map<vector<string>,int> group; // select列中的每一行数据完全一样时才会被记到一个key中 
	Database* tmp_database=DB.current_db;
	//cout<<word[count_index+3]<<endl;
//？？ 
	if(tmp_database!=NULL)
	for(auto i:tmp_database->table_list)
	{
		//cout<<i.first<<endl;
	}
	Table tmp_table=tmp_database->table_list[word[count_index+3]];//找到目前使用的table

	for(auto i:tmp_table.row_map) // 遍历当前 table 中的所有行 
	{
		//cout<<"!"<<endl;
		vector<string> tmp_row; // 每次抽出一个行，把所有数据 push 进 tmp_row 中 
		bool ok=1; 
		for(int j=0;j<basic.size();j++)
		{
			//cout<<"want to back"<<i.second.data[basic[j]]<<endl;
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
	//cout<<"index"<<index[0]<<endl; 	
	for(int j=1;j<count_index;j++)
	{
		cout<<word[j]<<'\t';
	}
	cout<<"COUNT("<<count_col<<")"<<endl;
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
	//if(DB.current_db!=NULL)cout<<"Count!!"<<endl;
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
	if(a1.type == "NULL")
		return a1 < a2;
	else if(a2.type == "NULL")
		return a2 < a1;
	else if(DB.current_db->table_list[table_name].row_map[a1].data[order_by_attr] == "NULL")
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

void NewSelect(string *word, int how_many_word, string wherestring, string order_by_attr) { // 用于处理 select + orderby 的情况 
	//cout<<"NewSelect\n";
	//cout<<"wherestring "<<wherestring<<endl<<"order_by_attr "<<order_by_attr<<endl;
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
			std::set<Data> key_of_rows;
			if(wherestring.find("LIKE") == -1) {
				StringSplit(wherestring, rootnode, &DB.current_db->table_list[table_name]);
				//std::set<Data> key_of_rows = where_clause(table_name, wherestring);
				key_of_rows = getWhereKeys(rootnode, &DB.current_db->table_list[table_name]);
			} 
			else
				key_of_rows = getWhereLikeKeys(&DB.current_db->table_list[table_name], wherestring);
				
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
			
			std::set<Data> key_of_rows;
			if(wherestring.find("LIKE") == -1) {
				StringSplit(wherestring, rootnode, &DB.current_db->table_list[table_name]);
				//std::set<Data> key_of_rows = where_clause(table_name, wherestring);
				key_of_rows = getWhereKeys(rootnode, &DB.current_db->table_list[table_name]);
			} 
			else
				key_of_rows = getWhereLikeKeys(&DB.current_db->table_list[table_name], wherestring);
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
	//cout<<"WHERESTRING!!! "<<wherestring<<endl;
	if(wherestring=="NULL"){
		for(auto i=table.row_map.begin();i!=table.row_map.end();i++){
			temp.clear();
			for(auto j:attrName){
				//cout<<"temp_pushback "<<i->second.data[j]<<" now temp size:["<<temp.size()<<"] attrname:"<<j<<endl;
				temp.push_back(i->second.data[j]);
			}
			selected.insert(temp);
		}
	} 
	else{
		set<Data> key_of_rows;
		if(wherestring.find("LIKE") != -1) {
			key_of_rows = getWhereLikeKeys(&table, wherestring); 
		}
		else {
			StringSplit(wherestring, rootnode, &table);
			key_of_rows=getWhereKeys(rootnode,&table);
		}
		//cout<<"key_of_rows"<<endl;
		for(auto i:key_of_rows){
			//cout<<"i "<<i.value<<endl;
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
				//cout<<"temp_pushback "<<i->second.data[j]<<" now temp size:["<<temp.size()<<"] attrname:"<<j<<endl;
				temp.push_back(i->second.data[j]);
			}
			multiselected.insert(temp);
		}
	}
	else{
		set<Data> key_of_rows;
		if(wherestring.find("LIKE") != -1) {
			key_of_rows = getWhereLikeKeys(&table, wherestring); 
		}
		else {
			StringSplit(wherestring, rootnode, &table);
			key_of_rows=getWhereKeys(rootnode,&table);
		}
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
	bool multi=false;//判断是否为"UNION ALL" 
	set<vector<string>> selected;
	multiset<vector<string>> multiselected;
	if(Find(word,"ALL",how_many_word)!=-1){
		multi=true;
	}
	string orderbyattrName;//用来排序的是哪一个（根据网上的说法名字按第一个表的表头名称走）
	int orderbynum;//是整个序列里的第几个（因为可能不同表格的表头名字不一样） 
	string orderbytype;//是什么类型 
	
	string* p;//指向某一个word的指针，每次从上次没查到的地方开始 
	vector<string> attrName;//可能有多个变量要找 
	string tableName;
	Table u_table;
	int pos;//"FROM"的位置
	int wherepos;
	string wherestr="NULL";//之后每次都要初始化
	
	p=word;
	pos=Find(p,"FROM",how_many_word);//如果用UNION的话，最后一个UNION后面的那张表的信息不好弄
	orderbyattrName=word[Find(p,"ORDER",how_many_word)+2];
	//cout<<orderbyattrName<<endl;
	for(int i=Find(p,"SELECT",how_many_word)+1;i<pos;i++){
		attrName.push_back(p[i]);
		//cout<<"attrname.push_back "<<word[i]<<endl;
		if(word[i]==orderbyattrName){
			orderbynum=i-Find(p,"SELECT",how_many_word)-1;
		}
	}
	tableName=p[pos+1];
	//cout<<tableName<<endl;
	orderbytype=GetOrderbyType(tableName,orderbyattrName);
	//cout<<orderbytype<<endl;
	u_table=DB.current_db->table_list[tableName];//因为要重新输出里面的元素所以拷贝构造一个新的没有关系
	//cout<<"word[pos+2] "<<word[pos+2]<<endl;
	if(word[pos+2]=="WHERE"){
		scmd=scmd.substr(scmd.find("WHERE")+6);
		int stop=scmd.find("UNION");
		if(stop==-1){
			stop=scmd.find("ORDER");
		}
		wherestr=scmd.substr(0,stop-1);//第二个参数是长度，不过是从0开始所以无所谓
		//cout<<"wherestr "<<wherestr<<endl; 
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
		attrName.clear();//可能表头的名称不一样，所以每次都要重新读 
		for(int i=Find(p,"SELECT",how_many_word)+1;i<pos;i++){
			attrName.push_back(p[i]);//看的是p的第几个！所以得是p[i]而非word[i] 
			//cout<<"attr.name_push_back "<<p[i]<<endl;
		}
		tableName=p[pos+1];//pos是对p的相对位置呀 
		u_table=DB.current_db->table_list[tableName];
		if(p[pos+2]=="WHERE"){
			scmd=scmd.substr(scmd.find("WHERE")+6);
			int stop=scmd.find("UNION");
			if(stop==-1){//如果有union的话一定会在order的前面 
				stop=scmd.find("ORDER");
			}
			wherestr=scmd.substr(0,stop-1);//第二个参数是长度，不过是从0开始所以无所谓 
			//cout<<"wherestr "<<wherestr<<endl;
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
		/*
		cout<<"selected"<<' '<<selected.size()<<endl;
		vector<string> tmp=*selected.begin();//检测重复元素
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
		*/
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
			selected.erase(minimum);//记得清除！
			if(!selected.empty()){//如果是空的就会出错 
				minimum=*selected.begin();//minimum也要改！不然还是清除前的最小值没办法换成新的值 
			}
		}
		//cout<<"success"<<endl;
		return;//记得return呀 
	}
	else{
		vector<string> minimum=*multiselected.begin();
		while(!multiselected.empty()){
			for(auto i:multiselected){
				if(UnionCompare(i,minimum,orderbytype,orderbynum)){
					minimum=i;
				}
			}
			for(auto i:minimum){
				cout<<i<<'\t';
			}
			cout<<endl;
			multiselected.erase(multiselected.find(minimum));//如果用multiselected.erase(一个值)那么同值的会全erase；而用迭代器则只会erase一个
			if(!multiselected.empty()){//multi!!! 
				minimum=*multiselected.begin();//minimum也要改！不然还是清除前的最小值没办法换成新的值 （记得把selected改成multiselected）
			}
		}
		return;
	}
}

void Join(string *word, int how_many_word, string wherestring, string order_by_attr) {
	//cout<<"Join\n";
//	cout<<"wherestring "<<wherestring<<endl<<"order_by_attr "<<order_by_attr<<endl;

	string order_by_table = order_by_attr.substr(0, order_by_attr.find('.'));
	order_by_attr = order_by_attr.substr(order_by_attr.find('.') + 1);

	int count_index = Find(word, "COUNT", how_many_word);
	int from_index = Find(word, "FROM", how_many_word);
	int join_index = Find(word, "JOIN", how_many_word);
	int on_index = Find(word, "ON", how_many_word);
	int where_index = Find(word, "WHERE", how_many_word);
	
	std::vector<std::string> table_name;	
	std::vector<std::string> attr_name;
	
	string str1 = word[on_index + 1].substr(0, word[on_index + 1].find('='));
	string str2 = word[on_index + 1].substr(word[on_index + 1].find('=') + 1);
	
	string MustExistTable;
	string table1 = str1.substr(0, str1.find('.'));
	string table2 = str2.substr(0, str2.find('.'));
	string column1 = str1.substr(str1.find('.') + 1);
	string column2 = str2.substr(str2.find('.') + 1);
	for(int i = 1; word[i] != "FROM"; i++) {
		table_name.push_back(word[i].substr(0, word[i].find('.'))); // 把所有 select 的列均放入 attr_name 这个 vector 里面备用 
		attr_name.push_back(word[i].substr(word[i].find('.') + 1));
	}
	for (auto i = attr_name.begin(); i < attr_name.end() - 1; i++) {
		std::cout << (*i) << "\t";
	}
	std::cout << *(attr_name.end()-1) << "\n" ;
	
	std::vector<Data> key_of_rows1, key_of_rows2;
	map<Data, bool> visited1, visited2; 
	
	for(auto it1 = DB.current_db->table_list[table1].row_map.begin(); it1 != DB.current_db->table_list[table1].row_map.end(); it1++)
		visited1[it1->first] = 0;
	for(auto it2 = DB.current_db->table_list[table2].row_map.begin(); it2 != DB.current_db->table_list[table2].row_map.end(); it2++)
		visited2[it2->first] = 0;

	for(auto it1 = DB.current_db->table_list[table1].row_map.begin(); it1 != DB.current_db->table_list[table1].row_map.end(); it1++) {
		for(auto it2 = DB.current_db->table_list[table2].row_map.begin(); it2 != DB.current_db->table_list[table2].row_map.end(); it2++) {
			//cout<<"Compare "<<it1->second.data[column1]<<" & "<<it2->second.data[column2]<<endl;
			if(it1->second.data[column1] == it2->second.data[column2]) {
				key_of_rows1.push_back(it1->first);
				key_of_rows2.push_back(it2->first);
				visited1[it1->first] = 1;
				visited2[it2->first] = 1;
			}
		}
	}

	Data nulldata;
	nulldata.type = "NULL";
	
	if(Find(word, "LEFT", how_many_word) != -1) 
		MustExistTable = word[from_index + 1];	
	if(Find(word, "RIGHT", how_many_word) != -1)
		MustExistTable = word[join_index + 1];
		
	if(MustExistTable != "") {
		if(MustExistTable == table1) {
			for(auto i = visited1.begin(); i != visited1.end(); i++)
				if(i->second == 0) {
					key_of_rows1.push_back(i->first);
					key_of_rows2.push_back(nulldata);
				}
		}
		if(MustExistTable == table2) {
			for(auto i = visited2.begin(); i != visited2.end(); i++)
				if(i->second == 0) {
					key_of_rows1.push_back(nulldata);
					key_of_rows2.push_back(i->first);
				}
		}
	}
	
	if(order_by_attr == "") {
		std::string value;
		for (int i = 0; i < key_of_rows1.size(); i++) {
			for (int j = 0; j < attr_name.size(); j++) {
				
				if(table_name[j] == table1 && key_of_rows1[i].type == "NULL")
					cout<<"NULL";
				else if(table_name[j] == table2 && key_of_rows2[i].type == "NULL")
					cout<<"NULL";
				else {
					if(table_name[j] == table1)
						value = DB.GetValue(table_name[j], attr_name[j], key_of_rows1[i].value);
					else if(table_name[j] == table2)
						value = DB.GetValue(table_name[j], attr_name[j], key_of_rows2[i].value);
					else 
						cout << "You join your horse huh???\n";
						
					std::string type = DB.GetType(table_name[j], attr_name[j]);
					OutputData(value, type);
				}
				
				std::cout << "\t";
			}
			cout<<endl;
		}
	}
	else {
		string tmp_type = GetOrderbyType(order_by_table, order_by_attr);
		
		while(!key_of_rows1.empty()) {
			
			int nowpos = 0;
			if(order_by_table == table1) {
				for (int i = 0; i < key_of_rows1.size(); i++) {
					if(OrderByCompare(table1, order_by_attr, key_of_rows1[i], key_of_rows1[nowpos], tmp_type))
						nowpos = i;
				}
			}
			
			if(order_by_table == table2) {
				for (int i = 0; i < key_of_rows2.size(); i++) {
					if(OrderByCompare(table2, order_by_attr, key_of_rows2[i], key_of_rows2[nowpos], tmp_type))
						nowpos = i;
				}
			}
			std::string value;
			for (int j = 0; j < attr_name.size(); j++) {
				
				if(table_name[j] == table1 && key_of_rows1[nowpos].type == "NULL")
					cout<<"NULL";
				else if(table_name[j] == table2 && key_of_rows2[nowpos].type == "NULL")
					cout<<"NULL";
				else {
					if(table_name[j] == table1)
						value = DB.GetValue(table_name[j], attr_name[j], key_of_rows1[nowpos].value);
					else if(table_name[j] == table2)
						value = DB.GetValue(table_name[j], attr_name[j], key_of_rows2[nowpos].value);
					else 
						cout << "You join your horse huh???\n";
						
					std::string type = DB.GetType(table_name[j], attr_name[j]);
					OutputData(value, type);
				}
				
				std::cout << "\t";
			}
			cout<<endl;
			
			key_of_rows1.erase(key_of_rows1.begin() + nowpos);
			key_of_rows2.erase(key_of_rows2.begin() + nowpos);
		}
	}
	
	return;
}
