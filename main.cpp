#include"Row.h"
#include"Table.h"
#include "Database.h"
#include "DatabaseMap.h"
#include "Command.h"
#include "clause_deal.h"

DatabaseMap DB;       
using namespace std;
int main()
{
	std::string command;
	char cmd[1200];
	while (cin.getline( cmd,1100, ';')){
		command=cmd;
		cout<<"comd="<<cmd<<endl;
	/*	if(command == "\n"){
			break;
		}*/
		Command a(command);
		a.operate();
		clause_deal(cmd,command);
	//	cout<<cmd<<endl;
	} 
}

