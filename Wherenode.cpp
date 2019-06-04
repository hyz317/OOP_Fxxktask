#include <algorithm>
#include "Wherenode.h"
#include "Table.h"
using namespace std;

bool Equal(string a,string b){
	transform(a.begin(),a.end(),a.begin(),::tolower);
	transform(b.begin(),b.end(),b.begin(),::tolower);
	return a==b;
}

void BuildWhereTree(Wherenode* node, string word[], int st, int ed, Table* mytable)
{
	// 现在默认是 AND 和 OR 两侧带空格， =<> 三种operator的运算不带空格。如果带的话需要改变处理方式
	if(st == ed) { // 如果只传进来一个string，那么意味着是一项表达式或者是一个被括号括起来的东西 
		if(word[st].find('(') != -1) {
			// 如果这个字串有括号，那么
			string str = word[st].substr(1, word[st].length() - 2); // 获得把括号去掉的字符串 
			StringSplit(str, node, mytable); // 把括号里面的语句再重新进行字符串分割 
			return;
		}
		else { // 如果没有括号，那么找到相应的 <>= 运算符，分割成俩串，处理后填入Wherenode的fieldname和data 
			if(word[st].find('=') != -1) {
				node->fieldname = word[st].substr(0, word[st].find('=')); //operator前面的子串是fieldname 
				string info = word[st].substr(word[st].find('=') + 1); //operator后面的子串是data的值（可能为int，string，double） 
				string type;// = mydatalist->field[node->fieldname]->type; //通过row中field获取真实的type 				
				for (auto iter = mytable->attr_list.begin(); iter != mytable->attr_list.end(); iter++)  //寰幆鍒ゆ柇宸﹀彸鏄惁涓哄睘鎬у悕
				{
					if(node->fieldname == iter->name)
						type = iter->type;			
				}				
				node->datatype = type;
				node->data = info;
				node->type = '=';
			}
			else if(word[st].find('<') != -1) {
				node->fieldname = word[st].substr(0, word[st].find('<'));
				string info = word[st].substr(word[st].find('<') + 1);
				string type;// = mydatalist->field[node->fieldname]->type; //通过row中field获取真实的type 				
				for (auto iter = mytable->attr_list.begin(); iter != mytable->attr_list.end(); iter++)  //寰幆鍒ゆ柇宸﹀彸鏄惁涓哄睘鎬у悕
				{
					if(node->fieldname == iter->name)
						type = iter->type;			
				}				
				node->datatype = type;
				node->data = info;
				node->type = '<';
			}
			else if(word[st].find('>') != -1) {
				node->fieldname = word[st].substr(0, word[st].find('>'));
				string info = word[st].substr(word[st].find('>') + 1);
				string type;// = mydatalist->field[node->fieldname]->type; //通过row中field获取真实的type 				
				for (auto iter = mytable->attr_list.begin(); iter != mytable->attr_list.end(); iter++)  //寰幆鍒ゆ柇宸﹀彸鏄惁涓哄睘鎬у悕
				{
					if(node->fieldname == iter->name)
						type = iter->type;			
				}				
				node->datatype = type;
				node->data = info;
				node->type = '>';
			}
		}
	}
	else { // 如果传进来一堆词，那么一定有 AND 和 OR
		//OR优先级低于AND，先处理OR【如果有两个OR就拿第一个OR当根，因为没有括号的话一堆OR先算哪个后算哪个是一样的】 
		
		for(int i=st; i<=ed; i++) {
			if(Equal(word[i],"xor")) {  //如果是OR 
				node->type = 'x';  
				node->next1 = new Wherenode; // new出两个新节点 
				node->next2 = new Wherenode;
				BuildWhereTree(node->next1, word, st, i-1, mytable); 
				BuildWhereTree(node->next2, word, i+1, ed, mytable);
				return;
			}
		}
		for(int i=st; i<=ed; i++) {
			if(Equal(word[i],"or")) {  //如果是OR 
				node->type = 'o';  
				node->next1 = new Wherenode; // new出两个新节点 
				node->next2 = new Wherenode;
				BuildWhereTree(node->next1, word, st, i-1, mytable); 
				BuildWhereTree(node->next2, word, i+1, ed, mytable);
				return;
			}
		}
		for(int i=st; i<=ed; i++) {
			if(Equal(word[i],"and")) {
				node->type = 'a';
				node->next1 = new Wherenode;
				node->next2 = new Wherenode;
				BuildWhereTree(node->next1, word, st, i-1, mytable);
				BuildWhereTree(node->next2, word, i+1, ed, mytable);
				return;
			}
		}
		for(int i=st; i<=ed; i++) {
			if(Equal(word[i],"not")) {  //如果是NOT 
				node->type = 'n';  
				node->next1 = new Wherenode; // new出一个新节点 
				BuildWhereTree(node->next1, word, i+1, ed, mytable);
				return;
			}
		}
	}
}

void StringSplit(string str, Wherenode* node, Table* mytable)
{
	/*
		新string分割方式：如果看到括号，把整个括号完了之前的东西都分成一个字符串！！
		比如 Sunday's coming (I wanna(drive my)) car
		分割为 [Sunday's] [coming] [(I wanna(drive my))] [car] 四个单词 
		其他词按照空格正常分隔
	*/
	string word[100];
	int pos = 0, startpos = 0, wordpos = 0;
	while(pos < str.length()) {
		if(str[pos] == ' ' || str[pos] == '\n') {
			if(startpos != pos)
				word[wordpos++] = str.substr(startpos, pos - startpos);
			pos++;
			startpos = pos;
		}
		else if(str[pos] == '(') {
			int bracket = 1;
			pos++;
			while(bracket) {
				if(str[pos] == '(')
					bracket++;
				if(str[pos] == ')')
					bracket--;
				pos++;
			}
		}
		else
			pos++;
	}
	word[wordpos++] = str.substr(startpos);
	BuildWhereTree(node, word, 0, wordpos-1, mytable); // 处理完了以后用分割好的字符串开始建树
}

bool compareless(const string &a1, const string &a2, string type)
{
	if(type == "int")
		return stoi(a1) < stoi(a2);
	else if(type == "double")
		return stod(a1) < stod(a2);
	else if(type == "char")
		return a1 < a2;
	else
		throw("you compare you horse huh"); 
}

bool comparemore(const string &a1, const string &a2, string type)
{
	if(type == "int")
		return stoi(a1) > stoi(a2);
	else if(type == "double")
		return stod(a1) > stod(a2);
	else if(type == "char")
		return a1 < a2;
	else
		throw("you compare you horse huh"); 
}

bool compareequal(const string &a1, const string &a2, string type)
{
	if(type == "int")
		return stoi(a1) == stoi(a2);
	else if(type == "double")
		return stod(a1) == stod(a2);
	else if(type == "char")
		return a1 == a2;
	else
		throw("you compare you horse huh"); 
}

bool JudgeWhere(Wherenode* node, Row* myrow) 
{
	if(node->type == 'a') { //如果是AND，递归处理两棵子树 
		return JudgeWhere(node->next1, myrow) && JudgeWhere(node->next2, myrow);
	}
	if(node->type == 'o') { //如果是OR，递归处理两棵子树 
		return JudgeWhere(node->next1, myrow) || JudgeWhere(node->next2, myrow);
	}
	if(node->type == 'n') { //如果是NOT，递归处理一棵子树 
		return !JudgeWhere(node->next1, myrow);
	}
	if(node->type == 'x') { //如果是XOR，递归处理两棵子树 
		return JudgeWhere(node->next1, myrow) != JudgeWhere(node->next2, myrow);
	}
	if(node->type == '<') { //如果是operator，直接判断真假并返回True或False 
		if(compareless(myrow->data[node->fieldname], node->data, node->datatype))
			return true;
		return false;
	}
	if(node->type == '>') {
		if(comparemore(myrow->data[node->fieldname], node->data, node->datatype))
			return true;
		return false;
	}
	if(node->type == '=') {
		if(compareequal(myrow->data[node->fieldname], node->data, node->datatype))
			return true;
		return false;
	}
}

std::set<Data> getWhereKeys(Wherenode *rootnode, Table *mytable)//适配器模式 
{
	std::set<Data> Dataset;
	for(auto i: mytable->row_map) {
		if(JudgeWhere(rootnode, &(i.second)))
			Dataset.insert(i.first);
	}
	return Dataset;
}
