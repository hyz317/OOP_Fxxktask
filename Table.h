#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include "Row.h"

struct Data {
	std::string value;
	std::string type;
	bool operator< (const Data d) const;
	bool operator== (const Data d)const;
};


class Table {
private:
	std::string key;     //主键，应为attribute中元素 
	std::string key_type;  
public:
	std::string getkeytype() { return key_type; }
	std::string getkey() { return key; }
	//std::string key;
	//std::map<std::string, Row> row_map;   //用string存关键字，用Row存这一行的数据 char
	//std::map<int, Row> row_map_int;
	//std::map<double, Row> row_map_double;
	std::map<Data, Row> row_map;
	std::vector<Attribute> attr_list;
	Table() {}
	Table(std::vector<Attribute> attr, std::string _key) :attr_list(attr),key(_key) { 
		//key.value = _key;
		for (auto it = attr.begin(); it < attr.end(); it++) {
			if ((*it).Key == 1) {
				key_type = (*it).type;
				break;
			}
		}
	}  //构造，我会把attr_list做好 //Table的第一个参数为name 删掉了
	void insert(std::vector<std::string> attr_name, std::vector<std::string> value);  //插入行  
	void OutputAttr();   //更新2：按输入顺序输出属性名，格式参考select *
	void find(const std::string key);
	std::set<Data> getallkeys();
	std::set<Data> keywherecluase(const Clause c);
};

