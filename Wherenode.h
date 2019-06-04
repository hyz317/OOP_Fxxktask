#pragma once
#include "Row.h"
#include "Table.h"
#include <string>

class Wherenode {
public:
	char type; //'a' => and, 'o' => or, '>' '<' '=' => operator
	//���type�� and �� or����ô����next1��next2��������ֵ�����㣬��ʱfieldname��dataӦ���ǿյ� 
	//���type������ operator����ô����fieldnameָ���data����Ҫ�Ƚϵ�data�ıȽ����㣬��ʱnext1��next2Ӧ���ǿյ� 
	std::string fieldname; 
	std::string data;
	std::string datatype;
	Wherenode *prev; 
	Wherenode *next1;  
	Wherenode *next2;
};

bool JudgeWhere(Wherenode*, Row*); // ���ڵݹ鴦�������� 
void BuildWhereTree(Wherenode*, std::string[], int, int, Table*); // ���ڴ��Ѿ��ָ�õ��ַ������� 
void StringSplit(std::string, Wherenode*, Table*); // ���ڷָ��ַ��� 
std::set<Data> getWhereKeys(Wherenode*, Table*);
bool compareless(const std::string&, const std::string&, std::string type);
bool comparemore(const std::string&, const std::string&, std::string type);
bool compareequal(const std::string&, const std::string&, std::string type);
