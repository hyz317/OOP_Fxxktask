#include <algorithm>
#include "Wherenode.h"
#include "Table.h"
using namespace std;

bool Equal(string a,string b){
	transform(a.begin(),a.end(),a.begin(),::tolower);
	transform(b.begin(),b.end(),b.begin(),::tolower);
	return a==b;
}

void setfieldname(Wherenode *&node, string word)
{
	if(word.find("LEFT") != -1) {
		node->fieldname = word.substr(5, word.length() - 6);
		node->func = "LEFT";
		return;
	}
	if(word.find("LCASE") != -1) {
		node->fieldname = word.substr(6, word.length() - 7);
		node->func = "LCASE";
		return;
	}
	if(word.find("UCASE") != -1) {
		node->fieldname = word.substr(6, word.length() - 7);
		node->func = "UCASE";
		return;
	}
	
	node->fieldname = word;
//	cout<<"FIELDNAME!!! "<<node->fieldname<<endl;
}

string getdata(Wherenode *&node, string word, string type)
{
//	cout<<"WORD??? "<<word<<endl;
	if(word.find("LEFT") != -1) {
		string subword = word.substr(5, word.length() - 6);
		int num = stoi(subword.substr(subword.find(',') + 1));	
		string info = subword.substr(0, subword.find(',') - 1);
		if(Equal(type, "char"))
			info = info.substr(1, info.length() - 2);
		if(num >= info.length()) return info;
//		cout<<"LEFT??? "<<info.substr(0, num)<<endl;
		return info.substr(0, num);
	}
	else if(word.find("LCASE") != -1) {
		word = word.substr(6, word.length() - 7);
		if(type == "char")
			word = word.substr(1, word.length() - 2);
		for(int i=0;i<word.length();i++) {
			if(word[i] >= 'A' && word[i] <= 'Z') word[i]+=32;
		}
//		cout<<"LCASE??? "<<word<<endl;
		return word;
	}
	else if(word.find("UCASE") != -1) {
		word = word.substr(6, word.length() - 7);
		if(type == "char")
			word = word.substr(1, word.length() - 2);
		for(int i=0;i<word.length();i++) {
			if(word[i] >= 'a' && word[i] <= 'z') word[i]-=32;
		}
//		cout<<"UCASE??? "<<word<<endl;
		return word;
	}
	
	if(type == "char")
		word = word.substr(1, word.length() - 2);
//	cout<<"DaTA!!!!! "<<word<<endl;
	return word;
}

void BuildWhereTree(Wherenode* node, string word[], int st, int ed, Table* mytable)
{
	// ����Ĭ���� AND �� OR ������ո� =<> ����operator�����㲻���ո�������Ļ���Ҫ�ı䴦��ʽ
	if(st == ed) { // ���ֻ������һ��string����ô��ζ����һ����ʽ������һ���������������Ķ��� 
		if(word[st][0] == '(') {
			// �������ִ������ţ���ô
			string str = word[st].substr(1, word[st].length() - 2); // ��ð�����ȥ�����ַ��� 
			StringSplit(str, node, mytable); // �������������������½����ַ����ָ� 
			return;
		}
		else { // ���û�����ţ���ô�ҵ���Ӧ�� <>= ��������ָ�����������������Wherenode��fieldname��data 
			if(word[st].find('=') != -1) {
				setfieldname(node, word[st].substr(0, word[st].find('='))); //operatorǰ����Ӵ���fieldname 
				string info = word[st].substr(word[st].find('=') + 1); //operator������Ӵ���data��ֵ������Ϊint��string��double�� 
				string type;// = mydatalist->field[node->fieldname]->type; //ͨ��row��field��ȡ��ʵ��type 				
				for (auto iter = mytable->attr_list.begin(); iter != mytable->attr_list.end(); iter++)  //循环判断左右是否为属性名
				{
					if(node->fieldname == iter->name)
						type = iter->type;	
					if(node->fieldname.find(',')!=-1 && node->fieldname.substr(0, node->fieldname.find(',')) == iter->name)
						type = iter->type;		
				}				
				node->datatype = type;
				node->data = getdata(node, info, type);
				node->type = '=';
			}
			else if(word[st].find('<') != -1) {
				setfieldname(node, word[st].substr(0, word[st].find('<')));
				string info = word[st].substr(word[st].find('<') + 1);
				string type;// = mydatalist->field[node->fieldname]->type; //ͨ��row��field��ȡ��ʵ��type 				
				for (auto iter = mytable->attr_list.begin(); iter != mytable->attr_list.end(); iter++)  //循环判断左右是否为属性名
				{
					if(node->fieldname == iter->name)
						type = iter->type;	
					if(node->fieldname.find(',')!=-1 && node->fieldname.substr(0, node->fieldname.find(',')) == iter->name)
						type = iter->type;		
				}				
				node->datatype = type;
				node->data = getdata(node, info, type);
				node->type = '<';
			}
			else if(word[st].find('>') != -1) {
				setfieldname(node, word[st].substr(0, word[st].find('>')));
				string info = word[st].substr(word[st].find('>') + 1);
				string type;// = mydatalist->field[node->fieldname]->type; //ͨ��row��field��ȡ��ʵ��type 				
				for (auto iter = mytable->attr_list.begin(); iter != mytable->attr_list.end(); iter++)  //循环判断左右是否为属性名
				{
					if(node->fieldname == iter->name)
						type = iter->type;	
					if(node->fieldname.find(',')!=-1 && node->fieldname.substr(0, node->fieldname.find(',')) == iter->name)
						type = iter->type;		
				}				
				node->datatype = type;
				node->data = getdata(node, info, type);
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
	else if(type == "char" || type == "date" || type == "time")
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
	else if(type == "char" || type == "date" || type == "time")
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
	else if(type == "char" || type == "date" || type == "time")
		return a1 == a2;
	else
		throw("you compare you horse huh"); 
}

string func(Wherenode *&node, string str, int num)
{
	if(node->func == "LEFT" && num != -1) {
		str = str.substr(0, num);
		return str;
	}
	if(node->func == "LCASE") {
		for(int i=0; i < str.length(); i++) {
			if(str[i] >= 'A' && str[i] <= 'Z') str[i]+=32;
		}
		return str;
	}
	if(node->func == "UCASE") {
		for(int i=0; i < str.length(); i++) {
			if(str[i] >= 'a' && str[i] <= 'z') str[i]-=32;
		}
		return str;
	}
	return str;
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
	int num = -1;
	string str1;
	if(node->fieldname.find(',')!=-1) {
		str1 = myrow->data[node->fieldname.substr(0, node->fieldname.find(','))];
		num = stoi(node->fieldname.substr(node->fieldname.find(',') + 1));
	}
	else
		str1 = myrow->data[node->fieldname];
	string str2 = node->data;
	str1 = func(node, str1, num);
	
	if(node->type == '<') { //�����operator��ֱ���ж���ٲ�����True��False 
		if(compareless(str1, str2, node->datatype))
			return true;
		return false;
	}
	if(node->type == '>') {
		if(comparemore(str1, str2, node->datatype))
			return true;
		return false;
	}
	if(node->type == '=') {
		if(compareequal(str1, str2, node->datatype))
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

bool Like(string a, string b)
{
//	cout<<"###"<<a<<'*'<<b<<'*'<<endl;
	if(a == "" && b == "") return true;
	if(a == "") return false; 
	if(a == "%") return true;
	if(b == "") return false;
	
	int p = 0;
	int least = 0;
	bool more = 0;
	while(a[p] == '%' || a[p] == '_') {
		if(a[p] == '%') {
			more = 1;
		}
		if(a[p] == '_') {
			least ++;
		}
		p++;
	}
	if(p == a.length() && more) {
		if(least <= b.length()) return true;
		return false;
	}
//	cout<<"more "<<more<<' '<<"least "<<least <<endl;
	b = b.substr(least);
	if(p != 0 && !more)
		return Like(a.substr(p), b);
	if(p != 0 && more) {
		int p2 = p;
		while(p2 < a.size() && a[p2] != '%' && a[p2] != '_')
			p2++;
		string sub = a.substr(p, p2-p);
//		cout<<"sub!:"<<sub<<";b:"<<b<<endl; 
		bool value = 0;
		while(b.find(sub) != -1) {
			//cout<<"sub=\""<<sub<<"\"\n";
			int pos = b.find(sub);
			b = b.substr(pos + sub.length());
			value = value || Like(a.substr(p2), b);
		}
		return value;
	}
	if(p == 0) {
		while(p < a.size() && a[p] != '%' && a[p] != '_')
			p++;
		string sub = a.substr(0, p);
		if(b.find(sub) == 0)
			return Like(a.substr(p), b.substr(sub.size()));
		else return false;
	}
	
} 

std::set<Data> getWhereLikeKeys(Table *mytable, string clause)
{
	
	int firstblankpos = clause.find(' ');
	int likepos = clause.find("LIKE");
	string column = clause.substr(0, likepos-1);
	string likestring = clause.substr(likepos+5);
	likestring = likestring.substr(1, likestring.length() - 2); 
//	cout<<"COLUMN!!! "<<column<<' '<<likestring<<endl;
	
	std::set<Data> Dataset;
	for(auto i: mytable->row_map) {
		if(Like(likestring, i.second.data[column]))
			Dataset.insert(i.first);
//		cout<<likestring<<' '<<i.second.data[column]<<endl;
	}
	return Dataset;
}
