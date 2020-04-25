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

CodeWriter::CodeWriter(string filename)
{
	labelnum=0;
	start=0;
	file=filename;
	filename+=".asm";
	fout.open(filename.c_str());
	if(!fout.is_open())
	{
		cerr << "ERROR: FILE " << filename << " COULD NOT BE OPENED" << endl;
	}
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

void CodeWriter::setFileName(string filename)
{
	if(fout.is_open()) fout.close();
	file=filename;
	filename+=".asm";
	fout.open(filename.c_str());
	if(!fout.is_open())
	{
		cerr << "ERROR: FILE " << filename << " COULD NOT BE OPENED" << endl;
	}
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
		fout << "D=M" << endl << "@R13" << endl << "A=M" << endl << "M=D" << endl;
	}
	else
	{
		fout << "@" << loc << endl << "D=M" << endl << "@" << index << endl << "D=D+A" << endl;
		fout << "@R13" << endl << "M=D" << endl << "@SP" << endl << "AM=M-1" << endl << "D=M" << endl;
		fout << "D=M" << endl << "@R13" << endl << "A=M" << endl << "M=D" << endl;
	}
}
