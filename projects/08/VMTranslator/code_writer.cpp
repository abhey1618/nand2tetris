#include "code_writer.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#define binary fout << "@SP" << endl << "AM=M-1" << endl << "D=M" << endl << "A=A-1" << endl
#define case_false fout << "D=0" << endl << "@label" << labelnum << endl << "0;JMP" << endl
#define case_true fout << "(label" << start << ")" << endl << "D=-1" << endl << "(label" << labelnum++ << ")" << endl
#define logical1 fout << "D=D-M" << endl << "@label" << labelnum++ << endl
//#define logical2 fout << "@SP" << endl << "AM=M-1" << endl << "A=A-1" << endl << "M=D" << endl
#define logical2 fout << "@SP" << endl << "A=M-1" << endl << "M=D" << endl
#define decs fout << "@SP" << endl << "M=M-1" << endl
#define incs fout << "@SP" << endl << "M=M+1" << endl
#define unary fout << "@SP" << endl << "A=M-1" << endl
#define callPush(label) fout << "@" << label << endl << "D=M" << endl << "@SP" << endl << "A=M" << endl << "M=D" << endl
#define returning fout << "@R14" << endl << "D=M" << endl << "A=D-1" << endl << "D=M" << endl
#define returnend fout << "M=D" << endl << "@R14" << endl << "M=M-1" << endl

CodeWriter::CodeWriter(string filename)
{
	labelnum=0;
	start=0;
	output_file=filename;
	filename+=".asm";
	fout.open(filename.c_str());
	if(!fout.is_open())
	{
		cerr << "ERROR: FILE " << filename << " COULD NOT BE OPENED" << endl;
	}
	//writeInit();
}

CodeWriter::CodeWriter()
{
	labelnum=0;
	start=0;
}

CodeWriter::~CodeWriter()
{
	if(fout.is_open()) fout.close();
}

void CodeWriter::Close()
{
	if(fout.is_open()) fout.close();
}

void CodeWriter::setFileName(string filename)
{
	//if(fout.is_open()) fout.close();
	file=filename;
	/*filename+=".asm";
	fout.open(filename.c_str());
	if(!fout.is_open())
	{
		cerr << "ERROR: FILE " << filename << " COULD NOT BE OPENED" << endl;
	}
	//writeInit();*/
}

void CodeWriter::writeArithmetic(string command)
{
	if (command == "add")
	{
		binary;
		fout << "M=D+M" << endl;
	}
	else if (command == "sub")
	{
		binary;
		fout << "M=M-D" << endl;
	}
	else if (command == "neg")
	{
		unary;
		fout << "M=-M" << endl;
	}
	else if (command == "not")
	{
		unary;
		fout << "M=!M" << endl;
	}
	else if (command == "and")
	{
		binary;
		fout << "M=D&M" << endl;
	}
	else if (command == "or")
	{
		binary;
		fout << "M=D|M" << endl;
	}
	else if (command == "eq")
	{
		binary;
		logical1;
		fout << "D;JEQ" << endl;
		case_false;
		case_true;
		logical2;
		start=labelnum;
	}
	else if (command == "gt")
	{
		binary;
		logical1;
		fout << "D;JLT" << endl;
		case_false;
		case_true;
		logical2;
		start=labelnum;
	}
	else if (command == "lt")
	{
		binary;
		logical1;
		fout << "D;JGT" << endl;
		case_false;
		case_true;
		logical2;
		start=labelnum;
	}
}

void CodeWriter::writePushPop(commands PushOrPop, string segment, int index)
{
	if(PushOrPop==C_PUSH)
	{
		if(segment=="constant")
		{
			fout << "@" << index << endl << "D=A" << endl << "@SP" << endl << "A=M" << endl << "M=D" << endl;
			incs;
		}
		else if (segment == "local") push("LCL",index);
		else if (segment == "argument") push("ARG",index);
		else if (segment == "this") push("THIS",index);
		else if (segment == "that") push("THAT",index);
		else if (segment == "pointer") push("3",index);
		else if (segment == "temp") push("5",index);
		else if (segment == "static")
		{
			fout << "@" << file << "." << index << endl << "D=M" << endl << "@SP" << endl << "A=M" << endl << "M=D" << endl;
			incs;
		}
	}
	else if(PushOrPop==C_POP)
	{
		if (segment == "local") pop("LCL",index);
		else if (segment == "argument") pop("ARG",index);
		else if (segment == "this") pop("THIS",index);
		else if (segment == "that") pop("THAT",index);
		else if (segment == "pointer") pop("3",index);
		else if (segment == "temp") pop("5",index);
		else if (segment == "static")
		{
			fout << "@" << file << "." << index << endl << "D=A" << endl << "@R13" << endl << "M=D" << endl << "@SP" << endl << "AM=M-1" << endl << "D=M" << endl;
			fout << "@R13" << endl << "A=M" << endl << "M=D" << endl;
		}
	}
}

void CodeWriter::push(string loc, int index)
{
	if(loc == "3" || loc == "5")
	{
		fout << "@" << loc << endl << "D=A" << endl << "@" << index << endl << "A=D+A" << endl << "D=M" << endl << "@SP" << endl << "A=M" << endl << "M=D" << endl;
		incs;
	}
	else
	{
		fout << "@" << loc << endl << "D=M" << endl << "@" << index << endl << "A=D+A" << endl << "D=M" << endl << "@SP" << endl << "A=M" << endl << "M=D" << endl;
		incs;
	}
}

void CodeWriter::pop(string loc, int index)
{
	if(loc == "3" || loc == "5")
	{
		fout << "@" << loc << endl << "D=A" << endl << "@" << index << endl << "D=D+A" << endl;
		fout << "@R13" << endl << "M=D" << endl << "@SP" << endl << "AM=M-1" << endl << "D=M" << endl;
		fout << "@R13" << endl << "A=M" << endl << "M=D" << endl;
	}
	else
	{
		fout << "@" << loc << endl << "D=M" << endl << "@" << index << endl << "D=D+A" << endl;
		fout << "@R13" << endl << "M=D" << endl << "@SP" << endl << "AM=M-1" << endl << "D=M" << endl;
		fout << "@R13" << endl << "A=M" << endl << "M=D" << endl;
	}
}

void CodeWriter::writeInit()
{
	fout << "@256" << endl << "D=A" << endl << "@SP" << endl << "M=D" << endl;
	writeCall("Sys.init",0);
}

void CodeWriter::writeLabel(string label)
{
	fout << "(" << label << ")" << endl;
}

void CodeWriter::writeGoto(string label)
{
	fout << "@" << label << endl;
	fout << "0;JMP" << endl;
}

void CodeWriter::writeIf(string label)
{
	binary;
	fout << "@" << label << endl << "D;JNE" << endl;
}

void CodeWriter::writeCall(string function_name, int num_arg)
{
	//writeLabel("label"+labelnum.to_string());
	//writePushPop(C_PUSH,constant,labelnum);
	string retAddress = function_name+to_string(labelnum);
	labelnum++;
	start=labelnum;

	fout << "@" << retAddress << endl << "D=A" << endl << "@SP" << endl << "A=M" << endl << "M=D" << endl;
	incs;
	callPush("LCL");
	incs;
	callPush("ARG");
	incs;
	callPush("THIS");
	incs;
	callPush("THAT");
	incs;

	//Arg=sp-n-5
	fout << "@SP" << endl << "D=M" << endl << "@" << num_arg << endl << "D=D-A" << endl <<"@5" << endl << "D=D-A" << endl << "@ARG" << endl << "M=D" << endl;
	//LCL = sp
	fout << "@SP" << endl << "D=M" << endl << "@LCL" << endl << "M=D" << endl;
	writeGoto(function_name);
	writeLabel(retAddress);
}

void CodeWriter::writeReturn()
{
	fout << "@LCL" << endl << "D=M" << endl << "@R14" << endl << "M=D" << endl;
	fout << "@5" << endl << "A=D-A" << endl << "D=M" << endl << "@R15" << endl << "M=D" << endl;

	pop("ARG",0);
	fout << "@ARG" << endl << "D=M+1" << endl << "@SP" << endl << "M=D" << endl;

	returning;
	fout << "@THAT" << endl;
	returnend;
	returning;
	fout << "@THIS" << endl;
	returnend;
	returning;
	fout << "@ARG" << endl;
	returnend;
	returning;
	fout << "@LCL" << endl;
	returnend;

	fout << "@R15" << endl << "A=M" << endl << "0;JMP" << endl;
}

void CodeWriter::writeFunction(string function_name, int num_local)
{
	writeLabel(function_name);
	while(num_local>0)
	{
		writePushPop(C_PUSH,"constant",0);
		num_local--;
	}
}

void CodeWriter::writeCurrentCommand(string command)
{
	fout << "//" << command << endl;
}

void CodeWriter::setOutputFile(string filename)
{
	labelnum=0;
	start=0;
	output_file=filename;
	filename+=".asm";
	fout.open(filename.c_str());
	if(!fout.is_open())
	{
		cerr << "ERROR: FILE " << filename << " COULD NOT BE OPENED" << endl;
	}
}
