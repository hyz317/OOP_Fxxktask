#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <algorithm>
#include <iterator>
#include "Command.h"
#include "DatabaseMap.h"
#include "Wherenode.h" 
using std::string;
using namespace std;
extern bool starting;

void Command::operate() {
	std::stringstream ss(command);
	std::string cmd_type;
//	cout<<__FUNCTION__<<"cmd:"<<command<<command<<endl;
	ss >> cmd_type;
//	string word2;
//	ss >> word2;//µ⁄∂˛∏ˆµ•¥  
	if (cmd_type == "CREATE" || cmd_type == "create") {
		Create(ss);
	}
	else if (cmd_type == "USE" || cmd_type == "use") {
		Use(ss);
	}
	else if (cmd_type == "DROP" || cmd_type == "drop") {
		Drop(ss);
	}
	else if (cmd_type == "SHOW" || cmd_type == "show") {
		Show(ss);
	}
	else if (cmd_type == "UPDATE" || cmd_type == "update") {
		Update(ss);
	}
	else if (cmd_type == "INSERT" || cmd_type == "insert") {
		Insert(ss);
	}
	else if (cmd_type == "DELETE" || cmd_type == "delete") {
		Delete(ss);
	}
	else if ((cmd_type == "SELECT" || cmd_type == "select")&&command.find("OUTFILE")==-1&&command.find("GROUP")==-1&&command.find("ORDER")==-1&&command.find("UNION")==-1&&command.find("JOIN")==-1) {	
		if(command.find("LEFT")!=-1||command.find("LCASE")!=-1||command.find("UCASE")!=-1||command.find("SPACE")!=-1||command.find("LOCATE")!=-1
		||command.find("STRCMP")!=-1||command.find("DIV")!=-1||command.find("MOD")!=-1||ss.str().substr(ss.str().find(' ')+1).find(' ')==-1)
		return ;
		//cout<<ss.str()<<endl;
		Select(ss);
	}
	else {
		//std::cout << "Wrong command!\n";
	}
}

void trim(std::string& s)
 {
    if (!s.empty())
    {
        s.erase(0, s.find_first_not_of(" "));
        s.erase(s.find_last_not_of(" ") + 1);
    }
}
void Create(std::stringstream& ss) {
	std::string obj;
	ss >> obj;
	if (obj == "DATABASE" || obj == "database") {
		std::string name;
		ss >> name;
		DB.CreateDatabase(name);
	}
	else if (obj == "TABLE" || obj == "table") {
		std::string name;
		getline(ss, name, '(');
		trim(name);
		//ss.ignore(1);
		std::vector<Attribute> attr;
		std::string _key;
		while (true) {
			Attribute tmp;
			ss >> tmp.name;
			if (tmp.name == ",")
				continue;
			if (tmp.name == ")") {
				break;
			}
			if (tmp.name == "PRIMARY" || tmp.name == "primary") {
				ss.get();
				char tmp2[4];
				ss.get(tmp2, 4);
				if (strcmp(tmp2, "KEY") == 0 || strcmp(tmp2, "key") == 0) {
					ss.ignore(1);
					getline(ss, _key, ')');
					//ss.ignore(1);
					continue;
				}
				else {
					ss.putback(tmp2[2]);
					ss.putback(tmp2[1]);
					ss.putback(tmp2[0]);
				}
			}
			std::string tmp3;
			ss >> tmp3;
			if (tmp3[tmp3.length() - 1] == ',') {
				tmp3.erase(tmp3.end() - 1);
				if (tmp3 == "INT" || tmp3 == "int") {
					tmp.type = "int";
				}
				else if (tmp3 == "CHAR" || tmp3 == "char") {
					tmp.type = "char";
				}
				else if (tmp3 == "DOUBLE" || tmp3 == "double") {
					tmp.type = "double";
				}
				else if (tmp3 == "DATE" || tmp3 == "date") {
					tmp.type = "date";
				}
				else if (tmp3 == "TIME" || tmp3 == "time") {
					tmp.type = "time";
				}
				else {
					//std::cout << "ERROR!\n";
					return;
				}
				attr.push_back(tmp);
			}
			else if (tmp3[tmp3.length() - 1] == ')') {
				tmp3.erase(tmp3.end() - 1);
				if (tmp3 == "INT" || tmp3 == "int") {
					tmp.type = "int";
				}
				else if (tmp3 == "CHAR" || tmp3 == "char") {
					tmp.type = "char";
				}
				else if (tmp3 == "DOUBLE" || tmp3 == "double") {
					tmp.type = "double";
				}
				else if (tmp3 == "DATE" || tmp3 == "date") {
					tmp.type = "date";
				}
				else if (tmp3 == "TIME" || tmp3 == "time") {
					tmp.type = "time";
				}
				else {
					//std::cout << "ERROR!\n";
					return;
				}
				attr.push_back(tmp);
				break;
			}
			else {
				if (tmp3 == "INT" || tmp3 == "int") {
					tmp.type = "int";
				}
				else if (tmp3 == "CHAR" || tmp3 == "char") {
					tmp.type = "char";
				}
				else if (tmp3 == "DOUBLE" || tmp3 == "double") {
					tmp.type = "double";
				}
				else if (tmp3 == "DATE" || tmp3 == "date") {
					tmp.type = "date";
				}
				else if (tmp3 == "TIME" || tmp3 == "time") {
					tmp.type = "time";
				}
				else {
					//std::cout << "ERROR!\n";
					return;
				}
				//ÂæÖ‰øÆÊîπ
				std::string tmp4;
				ss >> tmp4;    //NOT
				ss >> tmp4;    //NULL) NULL,
				tmp.Null = 0;
				//if (strcmp(tmp4, "NOT NULL") == 0 || strcmp(tmp4, "not null") == 0) {
				//	tmp.Null = 0;
				//}
				//else {
				//	std::cout << "ERROR!\n";
				//	return;
				//}
				attr.push_back(tmp);
				//char tmp5;
				//ss.get(tmp5);
				if (*(tmp4.end()-1)==')')
					break;
				else
					continue;
			}
		}
		for (auto i = attr.begin(); i < attr.end(); i++) {
			if (i->name == _key) {
				i->Key = true;
				break;
			}
		}
		DB.CreateTable(name, attr, _key);
	}
	else {
	//std::cout << "ERROR!\n";
	return;
	}
}

void Use(std::stringstream& ss) {
	std::string name;
	ss >> name;
	DB.UseDatabase(name);
}

void Drop(std::stringstream& ss) {
	std::string obj;
	ss >> obj;
	if (obj == "DATABASE" || obj == "database") {
		std::string name;
		ss >> name;
		DB.DropDatabase(name);
	}
	else if (obj == "TABLE" || obj == "table") {
		std::string name;
		ss >> name;
		DB.DropTable(name);
	}
	else {
		//std::cout << "ERROR!\n";
	}
}

void Show(std::stringstream& ss) {
	std::string obj;
	ss >> obj;
	if (obj == "DATABASES" || obj == "databases") {
		DB.ShowDatabases();
	}
	else if (obj == "TABLES" || obj == "tables") {
		DB.ShowTables();
	}
	else if (obj == "COLUMNS" || obj == "columns") {
		std::string tmp;
		std::string name;
		ss >> tmp;
		ss >> name;
		DB.ShowColumns(name);
	}
	else {
		//std::cout << "ERROR!\n";
	}
}

std::set<Data> where_clause(std::string table_name, std::string clause) {
	//Ê≥®ÊÑèÔºöËøôÈáåÈªòËÆ§ÊØè‰∏™Êù°‰ª∂Âè•‰πãÂÜÖÊ≤°ÊúâÁ©∫Ê†º
	std::stringstream ss(clause);
	std::vector<Clause> conditions;
	std::vector<std::string> operators;
	std::vector<std::set<Data>> keys;
	while (true) {
		Clause c;
		std::string str;
		ss >> str;
		int i = 0;
		for (; i < str.length(); i++) {
			if (str[i] == '=' || str[i] == '<' || str[i] == '>') {
				c.cmp_op = str[i];
				break;
			}
			
		}
		c.left = str.substr(0, i);
		if (c.left[0] == '"' || c.left[0] == '\'') {
			c.left = c.left.substr(1, c.left.length() - 2);
		}
		std::string right = str.substr(i + 1);
		//c.right = str.substr(i + 1);//ÈîôËØØÔºåÂè≥ÂÄºÂ≠óÁ¨¶‰∏≤‰∏∫"\"\a""
		if (right[0] == '"' || right[0] == '\'') {
			c.right = right.substr(1, right.length() - 2);
		}
		else
			c.right = str.substr(i + 1);
		conditions.push_back(c);
		keys.push_back(DB.KeyWhereCluase(table_name, c));
		std::string op;
		if (ss >> op) {
			operators.push_back(op);
		}
		else {
			break;
		}
	}
	auto it1 = keys.begin();
	auto it2 = operators.begin();
	while (it2 < operators.end()) {
		if ((*it2) == "AND" || (*it2) == "and") {
			std::set<Data> s;
			std::set_intersection((*it1).begin(), (*it1).end(), (*(it1 + 1)).begin(), (*(it1 + 1)).end(), std::inserter(s,s.begin()));
			*(it1 + 1) = s;
			it1 = keys.erase(it1);
			it2 = operators.erase(it2);
		}
		else if ((*it2) == "OR" || (*it2) == "or") {
			it1++;
			it2++;
		}
		else {
			//std::cout << "ERROR!\n";
		}
	}
	it1 = keys.begin();
	while (keys.size() > 1) {
		std::set<Data> s;
		std::set_union((*it1).begin(), (*it1).end(), (*(it1 + 1)).begin(), (*(it1 + 1)).end(), std::inserter(s, s.begin()));
		(*it1) = s;
		keys.erase(it1 + 1);
	}
	return keys[0];
}

void Update(std::stringstream& ss) {
	std::string table_name;
	ss >> table_name;
	
	std::string tmp;
	ss >> tmp;    //SET
	std::string attr_name;
	getline(ss, attr_name, '=');
	trim(attr_name);
	//ss.ignore(1);            //=
	std::string value;
	ss >> value;
	if (value[0] == '"'||value[0]=='\'') {
		value = value.substr(1, value.length() - 2);
	}
	ss >> tmp;     //WHERE
	std::string clause;
	getline(ss, clause, ';');
	trim(clause);
	std::set<Data> key_of_rows = where_clause(table_name,clause);
	for (auto i = key_of_rows.begin(); i != key_of_rows.end(); i++) {
		DB.Set(table_name, attr_name, value, (*i).value);
	}
	
	if(!starting){
		std::string filename="1"+DB.getname()+"+"+table_name+".txt";
		std::fstream fout(filename,std::ios::out);
		streambuf* ocb=cout.rdbuf();//origin cout buffer
		streambuf* fb=fout.rdbuf();//fout buffer
		cout.rdbuf(fb);
		DB.current_db->ShowColumns(table_name);
		cout<<"+++++"<<endl;
		Command a("select * from "+table_name);
		a.operate();
		fout.flush();
		fout.close();
		cout.rdbuf(ocb);
	}
}

void Insert(std::stringstream& ss) {
	std::string tmp;
	ss >> tmp;     //into
	std::string table_name;
	getline(ss, table_name, '(');
	trim(table_name);
	
	//ss.ignore(1);    //(
	std::vector<std::string> attr_list;
	while (true) {
		ss >> tmp;
		if (tmp[tmp.length() - 1] == ',') {
			tmp = tmp.substr(0, tmp.length() - 1);
			attr_list.push_back(tmp);
		}
		else if(tmp[tmp.length() - 1] == ')') {
			tmp = tmp.substr(0, tmp.length() - 1);
			attr_list.push_back(tmp);
			break;
		}
		else {
			//std::cout << "ERROR!\n";
			return;
		}
	}
	ss >> tmp;    //VALUE
	char tmp1;
	ss >> tmp1;    //(
	std::vector<std::string> value_list;
	while (true) {
		ss >> tmp;
		if (tmp[tmp.length() - 1] == ',') {
			tmp = tmp.substr(0, tmp.length() - 1);
			if (tmp[0] == '"') {
				tmp = tmp.substr(1, tmp.length() - 2);
			}
			value_list.push_back(tmp);
		}
		else if (tmp[tmp.length() - 1] == ')') {
			tmp = tmp.substr(0, tmp.length() - 1);
			if (tmp[0] == '"') {
				tmp = tmp.substr(1, tmp.length() - 2);
			}
			value_list.push_back(tmp);
			break;
		}
		else if (tmp[0] == '"') {
			tmp = tmp.substr(1);
			string part2;
			ss >> part2;
			if(part2[part2.length() - 1] == ',') {
				part2 = part2.substr(0, part2.length() - 2);
				tmp = tmp + " " + part2;
				value_list.push_back(tmp);
			}
			else if(part2[part2.length() - 1] == ')') {
				part2 = part2.substr(0, part2.length() - 2);
				tmp = tmp + " " + part2;
				value_list.push_back(tmp);
				break;
			}
		}
		else {
			//std::cout << "ERROR!\n";
			return;
		}
	}
	DB.InsertInto(table_name, attr_list, value_list);
	
	if(!starting){
		std::string filename="1"+DB.getname()+"+"+table_name+".txt";
		std::fstream fout(filename,std::ios::out);
		streambuf* ocb=cout.rdbuf();//origin cout buffer
		streambuf* fb=fout.rdbuf();//fout buffer
		cout.rdbuf(fb);
		DB.current_db->ShowColumns(table_name);
		cout<<"+++++"<<endl;
		Command a("select * from "+table_name);
		a.operate();
		fout.flush();
		fout.close();
		cout.rdbuf(ocb);
	}
}

void Delete(std::stringstream& ss) {
	std::string tmp;
	ss >> tmp;    //FROM
	std::string table_name;
	ss >> table_name;
	
	ss >> tmp;    //WHERE
	std::string clause;
	getline(ss, clause, ';');
	trim(clause);
	std::set<Data> key_of_rows = where_clause(table_name, clause);
	for (auto i = key_of_rows.begin(); i != key_of_rows.end(); i++) {
		DB.DeleteRow(table_name, (*i).value);
	}
	
	if(!starting){
		std::string filename="1"+DB.getname()+"+"+table_name+".txt";
		std::fstream fout(filename,std::ios::out);
		streambuf* ocb=cout.rdbuf();//origin cout buffer
		streambuf* fb=fout.rdbuf();//fout buffer
		cout.rdbuf(fb);
		DB.current_db->ShowColumns(table_name);
		cout<<"+++++"<<endl;
		Command a("select * from "+table_name);
		a.operate();
		fout.flush();
		fout.close();
		cout.rdbuf(ocb);
	}
}

void OutputData(std::string value, std::string type) {
	std::stringstream ss(value);
	if (value == "NULL") {
		std::cout << "NULL";
	}
	else if (type == "int"||type == "INT") {
		int tmp;
		ss >> tmp;
		std::cout << tmp;
	}
	else if (type == "double"||type == "DOUBLE") {
		double tmp;
		ss >> tmp;
		std::cout << tmp;
	}
	else if (type == "char"||type == "CHAR") {
		string tmp;
		ss >> tmp;
		std::cout << tmp;
	}
	else if (type == "date"||type == "DATE") {
		string tmp;
		ss >> tmp;
		std::cout << tmp;
	}
	else if (type == "time"||type == "TIME") {
		string tmp;
		ss >> tmp;
		std::cout << tmp;
		if(tmp[0] != '0') {
			ss >> tmp;
			std::cout << ' ' << tmp;
		}
	}
	else {
		//std::cout << "ERROR!\n";
	}
}
void Count(std::stringstream& ss){
	
}

void Select(std::stringstream& ss,bool foutput) {
	std::vector<std::string> attr_name;
	std::string tmp = "";
	ss >> tmp;
	if(!foutput){
		while (tmp!="from" && tmp!="FROM") { //∞—“™selectµƒattrNameÃÙ≥ˆ¿¥
			if(tmp.find("COUNT")!=-1){
				return;
			}
			attr_name.push_back(tmp);
			ss >> tmp;
		}
	}
	else{
		ss>>tmp;
		while (tmp!="into" && tmp!="INTO") { //∞—“™selectµƒattrNameÃÙ≥ˆ¿¥
			attr_name.push_back(tmp);
			ss >> tmp;
		}
		ss>>tmp>>tmp>>tmp;
	}
	//cout<<"attr"<<attr_name[0]<<endl; 
	/*
	if(attr_name[0].find("COUNT")!=-1)
	{
		int bg=attr_name[0].find("(");
		int ed=attr_name[0].find(")");
		
	}
	*/
	if (attr_name[0] == "*") {
		std::string table_name;
		ss >> table_name;
		tmp = "";
		ss >> tmp;
		if (tmp == "") {
			//table_name = table_name.substr(0, table_name.length() - 1);
			std::set<Data> key_of_rows = DB.GetAllKeys(table_name);
			if(key_of_rows.begin() != key_of_rows.end())
			if(!foutput){
				DB.OutputAttr(table_name);
			}
			for (auto i = key_of_rows.begin(); i != key_of_rows.end(); i++) {
				DB.OutputRow(table_name, (*i).value);
			}
			return;
		}
		else {
		//	ss >> tmp;   //WHERE
			std::string clause;
			getline(ss, clause, ';');
			trim(clause);
			std::set<Data> key_of_rows;
			if(clause.find("LIKE") == -1)
				key_of_rows = where_clause(table_name, clause);
			else
				key_of_rows = getWhereLikeKeys(&(DB.current_db->table_list[table_name]), clause);
			if(!foutput){
				DB.OutputAttr(table_name);
			}
			for (auto i = key_of_rows.begin(); i != key_of_rows.end(); i++) {
				DB.OutputRow(table_name, (*i).value);
			}
			return;
		}

	}
	else {
		std::string table_name;
		ss >> table_name;
		tmp = "";
		ss >> tmp;
		if (tmp == "") {
			//table_name = table_name.substr(0, table_name.length() - 1);
			if(!foutput){
				for (auto i = attr_name.begin(); i < attr_name.end() - 1; i++) {
					std::cout << (*i) << "\t";
				}
				std::cout << *(attr_name.end()-1) << "\n" ;
			}
			std::set<Data> key_of_rows = DB.GetAllKeys(table_name);
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
			return;
		}
		else {
			if(!foutput){
				for (auto i = attr_name.begin(); i < attr_name.end() - 1; i++) {
					std::cout << (*i) << "\t";
				}
				std::cout << *(attr_name.end()-1) << "\n" ;
			}
			//ss >> tmp;   //WHERE
			std::string clause;
			getline(ss, clause, ';');
			trim(clause);
			Wherenode *rootnode = new Wherenode; 
			std::set <Data> key_of_rows;
			if(clause.find("LIKE") == -1) {
				StringSplit(clause, rootnode, &DB.current_db->table_list[table_name]);
				//std::set<Data> key_of_rows = where_clause(table_name, wherestring);
				key_of_rows = getWhereKeys(rootnode, &DB.current_db->table_list[table_name]);
			} 
			else
				key_of_rows = getWhereLikeKeys(&DB.current_db->table_list[table_name], clause);
			for (auto i = key_of_rows.begin(); i != key_of_rows.end(); i++) {
				for (int j = 0; j < attr_name.size() - 1; j++) {
					std::string value = DB.GetValue(table_name, attr_name[j], (*i).value);
					std::string type = DB.GetType(table_name, attr_name[j]);
					OutputData(value, type);
					std::cout << "\t";
				}
				std::string value = DB.GetValue(table_name, attr_name[attr_name.size() - 1], (*i).value);
				std::string type = DB.GetType(table_name, attr_name[attr_name.size() - 1]);
				OutputData(value, type);
				std::cout << "\n";
			}
		}
	}
}
