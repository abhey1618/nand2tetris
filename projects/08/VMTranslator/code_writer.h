#ifndef __codewriter__
#define __codewriter__

#include "parser.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class CodeWriter
{
private:
	string file;
	string output_file;
	string function;
	int labelnum;
	int start;
	ofstream fout;

public:
	CodeWriter(string filename);
	CodeWriter();

	~CodeWriter();

	void setFileName(string filename);
	void setOutputFile(string filename);
	void writeArithmetic(string command);
	void writePushPop(commands PushOrPop, string segment, int index);
	void writeInit();
	void writeLabel(string label);
	void writeGoto(string label);
	void writeIf(string label);
	void writeCall(string function_name, int num_arg);
	void writeFunction(string function_name, int num_local);
	void writeReturn();
	void writeCurrentCommand(string command);

	void push(string loc, int index);
	void pop(string loc, int index);
	void Close();
};

#endif