#include"Row.h"
#include"Table.h"
#include <ctime>

std::string getAttrType(Table* ta, const std::string &str)
{
	for(auto i = 0; i < ta->attr_list.size(); i++) {
		if(ta->attr_list[i].name == str)
			return ta->attr_list[i].type;
	}
}

void MakeValidDate(std::string &str)
{
	int pos1 = str.find('-');
	int pos2 = str.substr(pos1+1).find('-') + pos1 + 1;
	int year = stoi(str.substr(0, pos1));
	int month = stoi(str.substr(pos1 + 1, pos2));
	int day = stoi(str.substr(pos2 + 1));
	struct tm t1 = { 0 };
	t1.tm_year = year - 1900; t1.tm_mon = month; t1.tm_mday = day;
	mktime(&t1);
	std::string newstr;
	newstr += std::to_string(t1.tm_year + 1900);
	newstr += '-';
	if(t1.tm_mon < 10) newstr += '0';
	newstr += std::to_string(t1.tm_mon);
	newstr += '-';
	if(t1.tm_mday < 10) newstr += '0';
	newstr += std::to_string(t1.tm_mday);
	str = newstr;
}

void MakeValidTime(std::string &str)
{
	int pos = str.find(' ');
	std::string str1 = str.substr(0, pos);
	std::string str2 = str.substr(pos+1);
	int pos1 = str1.find('-');
	int pos2 = str1.substr(pos1+1).find('-') + pos1 + 1;
	int pos3 = str2.find(':');
	int pos4 = str2.substr(pos3+1).find(':') + pos3 + 1;
	
	int year = stoi(str1.substr(0, pos1));
	int month = stoi(str1.substr(pos1 + 1, pos2));
	int day = stoi(str1.substr(pos2 + 1));
	int hour = stoi(str2.substr(0, pos3));
	int minute = stoi(str2.substr(pos3 + 1, pos4));
	int second = stoi(str2.substr(pos4 + 1));
	struct tm t1 = { 0 };
	t1.tm_year = year - 1900; t1.tm_mon = month; t1.tm_mday = day;
	t1.tm_hour = hour; t1.tm_min = minute; t1.tm_sec = second;
	mktime(&t1);
	
	std::string newstr;
	newstr += std::to_string(t1.tm_year + 1900);
	newstr += '-';
	if(t1.tm_mon < 10) newstr += '0';
	newstr += std::to_string(t1.tm_mon);
	newstr += '-';
	if(t1.tm_mday < 10) newstr += '0';
	newstr += std::to_string(t1.tm_mday);
	
	newstr += ' ';
	
	if(t1.tm_hour < 10) newstr += '0';
	newstr += std::to_string(t1.tm_hour);
	newstr += ':';
	if(t1.tm_min < 10) newstr += '0';
	newstr += std::to_string(t1.tm_min);
	newstr += ':';
	if(t1.tm_sec < 10) newstr += '0';
	newstr += std::to_string(t1.tm_sec);
	
	str = newstr;
}

bool JudgeValidDate(std::string &str)
{
	int pos1 = str.find('-');
	int pos2 = str.substr(pos1+1).find('-') + pos1 + 1;
	//std::cout<<"pos1 "<<pos1<<" pos2 "<<pos2<<std::endl;
	if(pos1 == -1 || pos2 == pos1 || pos2 == str.length() - 1) {
		return false;
	}
	for(int i=0;i<pos1;i++) {
		if(!isdigit(str[i])) {
			return false;
		}
	}
	for(int i=pos1+1;i<pos2;i++) {
		if(!isdigit(str[i])) {
			return false;
		}
	}
	for(int i=pos2+1;i<str.length();i++) {
		if(!isdigit(str[i])) {
			return false;
		}
	}
	MakeValidDate(str);
	//std::cout<<str<<" Judgetrue\n";
	return true;
}

bool JudgeValidTime(std::string &str)
{
	int pos = str.find(' ');
	std::string str1 = str.substr(0, pos);
	std::string str2 = str.substr(pos+1);
	int pos1 = str1.find('-');
	int pos2 = str1.substr(pos1+1).find('-') + pos1 + 1;
	int pos3 = str2.find(':');
	int pos4 = str2.substr(pos3+1).find(':') + pos3 + 1;
	if(pos1 == -1 || pos2 == pos1 || pos2 == str1.length() - 1) {
		return false;
	}
	if(pos3 == -1 || pos4 == pos3 || pos4 == str2.length() - 1) {
		return false;
	}
	for(int i=0;i<pos1;i++) {
		if(!isdigit(str1[i])) {
			return false;
		}
	}
	for(int i=pos1+1;i<pos2;i++) {
		if(!isdigit(str1[i])) {
			return false;
		}
	}
	for(int i=pos2+1;i<str1.length();i++) {
		if(!isdigit(str1[i])) {
			return false;
		}
	}
	for(int i=0;i<pos3;i++) {
		if(!isdigit(str2[i])) {
			return false;
		}
	}
	for(int i=pos3+1;i<pos4;i++) {
		if(!isdigit(str2[i])) {
			return false;
		}
	}
	for(int i=pos4+1;i<str2.length();i++) {
		if(!isdigit(str2[i])) {
			return false;
		}
	}
	MakeValidTime(str);
	return true;
}

void Row::into(Table* ta, std::vector<std::string> attr_name, std::vector<std::string> value) {

	table = ta;
	for (auto i = 0; i < ta->attr_list.size(); i++)  //构建横向为空的一行数据
	{
		std::string a = ta->attr_list[i].name;
		data.insert(std::pair<std::string, std::string>(a, "NULL"));
	}

	for (auto i = 0; i < attr_name.size(); i++) //为对应空格赋值；
	{
		std::string a = attr_name[i];
		std::string b = value[i];
		std::string type = getAttrType(ta, attr_name[i]);
		if(type != "date" && type != "time")
			data[a] = b;
		else if(type == "date") {
			if(JudgeValidDate(b))
				data[a] = b;
			else
				data[a] = "0DATE";
		}
		else if(type == "time") {
			if(JudgeValidTime(b))
				data[a] = b;
			else
				data[a] = "0TIME";
		}
	}
}

void Row::outputrow(Table* ta)
{
	table = ta;
	for (auto i = 0; i <ta->attr_list.size(); i++) 
	{
		std::string a = ta->attr_list[i].name;
		std::string type = ta->attr_list[i].type;
		OutputData(data[a], type);
		if (i < ta->attr_list.size() - 1)
			std::cout << "\t";
	}
	std::cout << "\n";
}
