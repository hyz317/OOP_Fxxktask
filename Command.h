#pragma once
#include <string>
#include <sstream>
#include "DatabaseMap.h"

extern DatabaseMap DB;

class Command {
private:
	std::string command;
public:
	Command(std::string c):command(c){}
	void operate();
};

void Create(std::stringstream& ss);
void Use(std::stringstream& ss);
void Drop(std::stringstream& ss);
void Show(std::stringstream& ss);
void Update(std::stringstream& ss);
void Insert(std::stringstream& ss);
void Delete(std::stringstream& ss);
void Select(std::stringstream& ss,bool foutput=false);
std::set<Data> where_clause(std::string, std::string);
//void Count(std::stringstream& ss);//tmd 
