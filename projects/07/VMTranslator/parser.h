#ifndef __PARSER__
#define __PARSER__

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

enum commands { C_ARITHMETIC, C_PUSH, C_POP, C_LABEL, C_GOTO, C_IF, C_FUNCTION, C_RETURN, C_CALL};

class Parser
{
private:
	string file;
	string current_command;
	ifstream fin;

public:
	Parser(string filename);//Constructor
	~Parser();//Destructor

	bool hasMoreCommands();
	void advance();
	commands commandType();
	string arg1();
	int arg2();
};

#endif
