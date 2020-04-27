#include <iostream>
#include <string>
#include "Parser.h"
#include "code_writer.h"
#include "Parser.cpp"
#include "code_writer.cpp"

using namespace std;

int main(int argc, char **argv)
{
	string filename,temp;
	int dotloc,i;
	temp=argv[1];
	CodeWriter cw(temp);
	filename = argv[2];
	dotloc = filename.find(".",0);
	filename = filename.substr(0,dotloc);
	//cw.setFileName(temp);
	if(argc>3||filename!=temp) cw.writeInit();
	for(i=2;i<argc;i++)
	{
		filename = argv[i];
		dotloc = filename.find(".",0);
		filename = filename.substr(0,dotloc);
		Parser prser(filename);
		cw.setFileName(filename);
		while(prser.hasMoreCommands())
		{
			prser.advance();
			cw.writeCurrentCommand(prser.getCommand());
			if(prser.commandType()==C_ARITHMETIC) cw.writeArithmetic(prser.arg1());
			else if (prser.commandType() == C_POP) cw.writePushPop(C_POP, prser.arg1(), prser.arg2());
			else if (prser.commandType() == C_PUSH) cw.writePushPop(C_PUSH, prser.arg1(), prser.arg2());
			else if (prser.commandType() == C_LABEL) cw.writeLabel(prser.arg1());
			else if (prser.commandType() == C_GOTO) cw.writeGoto(prser.arg1());
			else if (prser.commandType() == C_IF) cw.writeIf(prser.arg1());
			else if (prser.commandType() == C_CALL) cw.writeCall(prser.arg1(),prser.arg2());
			else if (prser.commandType() == C_FUNCTION) cw.writeFunction(prser.arg1(),prser.arg2());
			else if (prser.commandType() == C_RETURN) cw.writeReturn();
		}
	}
}
