#pragma once
#include "Row.h"
#include "Table.h"
#include <string>

class Wherenode {
public:
	char type; //'a' => and, 'o' => or, '>' '<' '=' => operator
	//如果type是 and 或 or，那么返回next1和next2的子树的值的运算，此时fieldname和data应该是空的 
	//如果type是三种 operator，那么返回fieldname指向的data与需要比较的data的比较运算，此时next1和next2应该是空的 
	std::string fieldname; 
	std::string data;
	std::string datatype;
	Wherenode *prev; 
	Wherenode *next1;  
	Wherenode *next2;
};

bool JudgeWhere(Wherenode*, Row*); // 用于递归处理整个树 
void BuildWhereTree(Wherenode*, std::string[], int, int, Table*); // 用于从已经分割好的字符串建树 
void StringSplit(std::string, Wherenode*, Table*); // 用于分割字符串 
std::set<Data> getWhereKeys(Wherenode*, Table*);
bool compareless(const std::string&, const std::string&, std::string type);
bool comparemore(const std::string&, const std::string&, std::string type);
bool compareequal(const std::string&, const std::string&, std::string type);
