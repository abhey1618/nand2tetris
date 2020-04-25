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
	int labelnum;
	int start;
	ofstream fout;

public:
	CodeWriter(string filename);
	CodeWriter();
	~CodeWriter();

	void setFileName(string filename);
	void writeArithmetic(string command);
	void writePushPop(commands PushOrPop, string segment, int index);
	void push(string loc, int index);
	void pop(string loc, int index);
	void Close();
};

#endif