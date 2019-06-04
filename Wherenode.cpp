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
	// ����Ĭ���� AND �� OR ������ո� =<> ����operator�����㲻���ո�������Ļ���Ҫ�ı䴦��ʽ
	if(st == ed) { // ���ֻ������һ��string����ô��ζ����һ����ʽ������һ���������������Ķ��� 
		if(word[st].find('(') != -1) {
			// �������ִ������ţ���ô
			string str = word[st].substr(1, word[st].length() - 2); // ��ð�����ȥ�����ַ��� 
			StringSplit(str, node, mytable); // �������������������½����ַ����ָ� 
			return;
		}
		else { // ���û�����ţ���ô�ҵ���Ӧ�� <>= ��������ָ�����������������Wherenode��fieldname��data 
			if(word[st].find('=') != -1) {
				node->fieldname = word[st].substr(0, word[st].find('=')); //operatorǰ����Ӵ���fieldname 
				string info = word[st].substr(word[st].find('=') + 1); //operator������Ӵ���data��ֵ������Ϊint��string��double�� 
				string type;// = mydatalist->field[node->fieldname]->type; //ͨ��row��field��ȡ��ʵ��type 				
				for (auto iter = mytable->attr_list.begin(); iter != mytable->attr_list.end(); iter++)  //循环判断左右是否为属性名
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
				string type;// = mydatalist->field[node->fieldname]->type; //ͨ��row��field��ȡ��ʵ��type 				
				for (auto iter = mytable->attr_list.begin(); iter != mytable->attr_list.end(); iter++)  //循环判断左右是否为属性名
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
				string type;// = mydatalist->field[node->fieldname]->type; //ͨ��row��field��ȡ��ʵ��type 				
				for (auto iter = mytable->attr_list.begin(); iter != mytable->attr_list.end(); iter++)  //循环判断左右是否为属性名
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
	else { // ���������һ�Ѵʣ���ôһ���� AND �� OR
		//OR���ȼ�����AND���ȴ���OR�����������OR���õ�һ��OR��������Ϊû�����ŵĻ�һ��OR�����ĸ������ĸ���һ���ġ� 
		
		for(int i=st; i<=ed; i++) {
			if(Equal(word[i],"xor")) {  //�����OR 
				node->type = 'x';  
				node->next1 = new Wherenode; // new�������½ڵ� 
				node->next2 = new Wherenode;
				BuildWhereTree(node->next1, word, st, i-1, mytable); 
				BuildWhereTree(node->next2, word, i+1, ed, mytable);
				return;
			}
		}
		for(int i=st; i<=ed; i++) {
			if(Equal(word[i],"or")) {  //�����OR 
				node->type = 'o';  
				node->next1 = new Wherenode; // new�������½ڵ� 
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
			if(Equal(word[i],"not")) {  //�����NOT 
				node->type = 'n';  
				node->next1 = new Wherenode; // new��һ���½ڵ� 
				BuildWhereTree(node->next1, word, i+1, ed, mytable);
				return;
			}
		}
	}
}

void StringSplit(string str, Wherenode* node, Table* mytable)
{
	/*
		��string�ָʽ������������ţ���������������֮ǰ�Ķ������ֳ�һ���ַ�������
		���� Sunday's coming (I wanna(drive my)) car
		�ָ�Ϊ [Sunday's] [coming] [(I wanna(drive my))] [car] �ĸ����� 
		�����ʰ��տո������ָ�
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
	BuildWhereTree(node, word, 0, wordpos-1, mytable); // ���������Ժ��÷ָ�õ��ַ�����ʼ����
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
	if(node->type == 'a') { //�����AND���ݹ鴦���������� 
		return JudgeWhere(node->next1, myrow) && JudgeWhere(node->next2, myrow);
	}
	if(node->type == 'o') { //�����OR���ݹ鴦���������� 
		return JudgeWhere(node->next1, myrow) || JudgeWhere(node->next2, myrow);
	}
	if(node->type == 'n') { //�����NOT���ݹ鴦��һ������ 
		return !JudgeWhere(node->next1, myrow);
	}
	if(node->type == 'x') { //�����XOR���ݹ鴦���������� 
		return JudgeWhere(node->next1, myrow) != JudgeWhere(node->next2, myrow);
	}
	if(node->type == '<') { //�����operator��ֱ���ж���ٲ�����True��False 
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

std::set<Data> getWhereKeys(Wherenode *rootnode, Table *mytable)//������ģʽ 
{
	std::set<Data> Dataset;
	for(auto i: mytable->row_map) {
		if(JudgeWhere(rootnode, &(i.second)))
			Dataset.insert(i.first);
	}
	return Dataset;
}
