#include "parser.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

Parser::Parser()
{
	file="";
}

Parser::Parser(string filename)
{
	file=filename;
	filename += ".vm";
	fin.open(filename.c_str());
	if(!fin.is_open())
	{
		cerr << "ERROR: FILE " << filename << " COULD NOT BE OPENED" << endl;
	}
}
//Destructor
Parser::~Parser()
{
	if(fin.is_open()) fin.close();
}

bool Parser::hasMoreCommands()
{
	return !fin.eof(); //tells us if we have reached end of file
}

void Parser::setFileName(string filename)
{
	file=filename;
	filename += ".vm";
	fin.open(filename.c_str());
	if(!fin.is_open())
	{
		cerr << "ERROR: FILE " << filename << " COULD NOT BE OPENED" << endl;
	}
}

void Parser::advance()
{
	int i;
	if(hasMoreCommands())
	{
		getline(fin, current_command);
		while((current_command.substr(0,2)=="//"||current_command.substr(0,1)=="")&&hasMoreCommands())
		{
			getline(fin, current_command);
		}
		//we have got a command which is neither blank nor comment
		//it may still have a comment though
		if(!hasMoreCommands()) return;
		int index=current_command.find("//",0);
		current_command=current_command.substr(0,index);
		while(current_command.substr(current_command.length()-1,1)==" ")
		{
			current_command=current_command.substr(0,current_command.length()-1);
		}
	}
}

commands Parser::commandType()
{
	if ((current_command.substr(0, 3) == "add") || (current_command.substr(0, 3) == "sub") || (current_command.substr(0, 3) == "neg") || (current_command.substr(0, 3) == "and") || (current_command.substr(0, 3) == "not") || (current_command.substr(0, 2) == "eq") || (current_command.substr(0, 2) == "gt") || (current_command.substr(0, 2) == "lt") || (current_command.substr(0, 2) == "or"))
		return C_ARITHMETIC;
	else if ((current_command.substr(0, 4) == "push"))
		return C_PUSH;
	else if ((current_command.substr(0, 3) == "pop"))
		return C_POP;
	else if ((current_command.substr(0, 5) == "label"))
		return C_LABEL;
	else if ((current_command.substr(0, 4) == "goto"))
		return C_GOTO;
	else if ((current_command.substr(0, 2) == "if"))
		return C_IF;
	else if ((current_command.substr(0, 8) == "function"))
		return C_FUNCTION;
	else if ((current_command.substr(0, 6) == "return"))
		return C_RETURN;
	else if ((current_command.substr(0, 4) == "call"))
		return C_CALL;
	//else cerr << "COMMAND NOT FOUND: " << current_command << endl;
	else cerr << endl;
}

string Parser::arg1()
{
	int first, second;
	string res;
	commands ctype = commandType();
	if(ctype==C_RETURN)
	{
		cerr << "arg1 function cannot be performed for Return type instruction" << endl;
	}
	first = current_command.find(" ",0);
	second = current_command.find(" ",first+1);
	res = current_command.substr(first+1, second-first-1);
	return res;
}

int Parser::arg2()
{
	int first, second, third, ans;
	string a2;
	commands ctype = commandType();
	if (ctype == C_PUSH || ctype == C_POP || ctype == C_FUNCTION || ctype == C_CALL)
	{
		first = current_command.find(" ", 0);
		second = current_command.find(" ", first + 1);
		third = current_command.find(" ", second + 1);
		a2 = current_command.substr(second + 1, third - second - 1);
		ans = stoi(a2);
		return ans;
	}
}

string Parser::getCommand()
{
	return current_command;
}