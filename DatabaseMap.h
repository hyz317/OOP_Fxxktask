#pragma once
#include <map>
#include <string>
#include "Database.h"
#include <utility>


class DatabaseMap {
private:
	std::map<std::string,Database> dbs;    
	std::string current_name;   
         
public:
	Database* current_db = nullptr;
	DatabaseMap(){};
	void CreateDatabase(std::string db_name);
	void DropDatabase (std::string db_name);
	void UseDatabase  (std::string db_name);
	void ShowDatabases();
	void InsertInto(std::string table_name, std::vector<std::string> attr_name, std::vector<std::string> value);
	void CreateTable(std::string table_name, std::vector<Attribute> attr, std::string _key);
	void DropTable(std::string table_name);     
	void ShowTables();     
	void ShowColumns(std::string table_name);    
	std::set<Data> GetAllKeys(const std::string table_name);//返回所有主键的集合
	std::set<Data> KeyWhereCluase(const std::string table_name, const Clause c);     //查找给定表中符合条件c的所有Row，返回这些Row主键的集合
	void DeleteRow(const std::string table_name, const std::string key);   //删除指定表中主键为key的行
	void Set(const std::string table_name, const std::string attr_name, const std::string value, const std::string key);   //将表tablename中主键为key的行中的attr属性值更新为value
	std::string GetValue(const std::string table_name, const std::string attr, const std::string key);    //返回指定表，key对应行，attr对应列的数据
	std::string GetType(const std::string table_name, const std::string attr);    //返回指定表attr列的类型
	void OutputAttr(const std::string table_name);    //调用指定表的OutputAttr
	void OutputRow(const std::string table_name, const std::string key);     //找到对应Row，调用Row的output
	std::string getkeytype(const std::string table_name);
};
