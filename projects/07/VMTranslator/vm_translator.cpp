#include <iostream>
#include <string>
#include "Parser.h"
#include "code_writer.h"
#include "Parser.cpp"
#include "code_writer.cpp"

using namespace std;

int main(int argc, char **argv)
{
	CodeWriter cw;
	string filename;
	int dotloc,i;
	for(i=1;i<argc;i++)
	{
		filename = argv[i];
		dotloc = filename.find(".",0);
		filename = filename.substr(0,dotloc);
		Parser prser(filename);
		cw.setFileName(filename);
		while(prser.hasMoreCommands())
		{
			prser.advance();
			if(prser.commandType()==C_ARITHMETIC) cw.writeArithmetic(prser.arg1());
			else if (prser.commandType() == C_POP) cw.writePushPop(C_POP, prser.arg1(), prser.arg2());
			else if (prser.commandType() == C_PUSH) cw.writePushPop(C_PUSH, prser.arg1(), prser.arg2());
		}
	}
}
