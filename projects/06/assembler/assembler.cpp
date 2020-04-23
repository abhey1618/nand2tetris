#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include <string.h>
#include <string>

using namespace std;

void to_binary(int n, string &res)
{
	int high = 16384;//2^14
	while (high > 0)
	{
		if (n >= high)
		{
			res += "1";
			n = n - high;
		}
		else res += "0";
		high /= 2;
	}
}

void convert_toC(string original, string& res)
{
	string dest, comp, jump, temp;
	int equalsign = original.find("=", 0),semi;
	temp = "";
	//int semisign = original.find(";", 0);
	if (equalsign < original.length())//equal sign exists in the command
	{
		temp = original.substr(0, equalsign);
		dest = "";
		if (temp.find("A") < temp.length()) dest += "1";
		else dest += "0";
		if (temp.find("D") < temp.length()) dest += "1";
		else dest += "0";
		if (temp.find("M") < temp.length()) dest += "1";
		else dest += "0";
	}
	else dest = "000";
	if (temp.length() == 0) equalsign = -1;//no equal to sign
	if (original.substr(equalsign + 1, 3) == "D|M")
		comp = "1010101";
	else if (original.substr(equalsign + 1, 3) == "M|D")
		comp = "1010101";
	else if (original.substr(equalsign + 1, 3) == "D|A")
		comp = "0010101";
	else if (original.substr(equalsign + 1, 3) == "A|D")
		comp = "0010101";
	else if (original.substr(equalsign + 1, 3) == "D&A")
		comp = "0000000";
	else if (original.substr(equalsign + 1, 3) == "A&D")
		comp = "0000000";
	else if (original.substr(equalsign + 1, 3) == "D&M")
		comp = "1000000";
	else if (original.substr(equalsign + 1, 3) == "M&D")
		comp = "1000000";
	else if (original.substr(equalsign + 1, 3) == "A-D")
		comp = "0000111";
	else if (original.substr(equalsign + 1, 4) == "-D+A")
		comp = "0000111";
	else if (original.substr(equalsign + 1, 3) == "M-D")
		comp = "1000111";
	else if (original.substr(equalsign + 1, 4) == "-D+M")
		comp = "1000111";
	else if (original.substr(equalsign + 1, 3) == "D+1")
		comp = "0011111";
	else if (original.substr(equalsign + 1, 3) == "A+1")
		comp = "0110111";
	else if (original.substr(equalsign + 1, 3) == "M+1")
		comp = "1110111";
	else if (original.substr(equalsign + 1, 3) == "D-1")
		comp = "0001110";
	else if (original.substr(equalsign + 1, 3) == "A-1")
		comp = "0110010";
	else if (original.substr(equalsign + 1, 3) == "M-1")
		comp = "1110010";
	else if (original.substr(equalsign + 1, 3) == "M-1")
		comp = "1110010";
	else if (original.substr(equalsign + 1, 3) == "D+A")
		comp = "0000010";
	else if (original.substr(equalsign + 1, 3) == "A+D")
		comp = "0000010";
	else if (original.substr(equalsign + 1, 3) == "D+M")
		comp = "1000010";
	else if (original.substr(equalsign + 1, 3) == "M+D")
		comp = "1000010";
	else if (original.substr(equalsign + 1, 3) == "D-A")
		comp = "0010011";
	else if (original.substr(equalsign + 1, 3) == "D-M")
		comp = "1010011";
	else if (original.substr(equalsign + 1, 2) == "-1")
		comp = "0111010";
	else if (original.substr(equalsign + 1, 2) == "!D")
		comp = "0001101";
	else if (original.substr(equalsign + 1, 2) == "!A")
		comp = "0110001";
	else if (original.substr(equalsign + 1, 2) == "!M")
		comp = "1110001";
	else if (original.substr(equalsign + 1, 2) == "-D")
		comp = "0001111";
	else if (original.substr(equalsign + 1, 2) == "-A")
		comp = "0110011";
	else if (original.substr(equalsign + 1, 2) == "-M")
		comp = "1110011";
	else if (original.substr(equalsign + 1, 1) == "0")
		comp = "0101010";
	else if (original.substr(equalsign + 1, 1) == "1")
		comp = "0111111";
	else if (original.substr(equalsign + 1, 1) == "D")
		comp = "0001100";
	else if (original.substr(equalsign + 1, 1) == "A")
		comp = "0110000";
	else if (original.substr(equalsign + 1, 1) == "M")
		comp = "1110000";
	semi = original.find(";", 0);
	if (semi < original.length())//jump is there
	{
		if (original.substr(semi + 1, 3) == "JGT")
			jump = "001";
		else if (original.substr(semi + 1, 3) == "JEQ")
			jump = "010";
		else if (original.substr(semi + 1, 3) == "JGE")
			jump = "011";
		else if (original.substr(semi + 1, 3) == "JLT")
			jump = "100";
		else if (original.substr(semi + 1, 3) == "JNE")
			jump = "101";
		else if (original.substr(semi + 1, 3) == "JLE")
			jump = "110";
		else if (original.substr(semi + 1, 3) == "JMP")
			jump = "111";
	}
	else jump = "000";
	res = res + comp + dest + jump;
}

int main(int argc, char* argv[])
{
	int i, j;
	ifstream myfile(argv[argc - 1]);
	string file = argv[argc - 1];
	int dot = file.find(".", 0);
	string intfile = file.substr(0, dot) + ".int";
	string hackfile = file.substr(0, dot) + ".hack";
	ofstream fint(intfile.c_str());
	ofstream fhack(hackfile.c_str());

	map<string, int> SymbolTable;
	vector<string> symbols;
	SymbolTable["SP"] = 0;
	SymbolTable["LCL"] = 1;
	SymbolTable["ARG"] = 2;
	SymbolTable["THIS"] = 3;
	SymbolTable["THAT"] = 4;
	SymbolTable["R0"] = 0;
	SymbolTable["R1"] = 1;
	SymbolTable["R2"] = 2;
	SymbolTable["R3"] = 3;
	SymbolTable["R4"] = 4;
	SymbolTable["R5"] = 5;
	SymbolTable["R6"] = 6;
	SymbolTable["R7"] = 7;
	SymbolTable["R8"] = 8;
	SymbolTable["R9"] = 9;
	SymbolTable["R10"] = 10;
	SymbolTable["R11"] = 11;
	SymbolTable["R12"] = 12;
	SymbolTable["R13"] = 13;
	SymbolTable["R14"] = 14;
	SymbolTable["R15"] = 15;
	SymbolTable["SCREEN"] = 16384;
	SymbolTable["KBD"] = 24576;

	for (auto itr2 = SymbolTable.begin(); itr2 != SymbolTable.end(); itr2++)
	{
		symbols.push_back(itr2->first);
	}

	int newkey = 16;
	int counter = 0;

	string command,newcomm;
	bool multi = false;//tells us if multiline commenting is on or not
	while (getline(myfile, command))
	{
		//remove comments
		newcomm = "";
		if (multi)
		{
			auto ender = command.find("*/", 0);
			//cout << ender << " " << command.length() << endl;
			if (ender < command.length() - 1)//found
			{
				multi = false;
				//command = command.substr(ender + 2, command.length() - ender - 2);
			}
			continue;
		}
		if (command.length() == 0) continue;
		while (command.substr(0, 1) == " " || command.substr(0, 1) == "\t")
		{
			command = command.substr(1, command.length() - 1);
		}
		if (command.substr(0, 2) == "//") continue;
		if (command.substr(0, 2) == "/*")
		{
			multi = true;
			continue;
		}
		if (command.substr(0, 1) == "(")
		{
			/*auto itr=SymbolTable.find(command.substr(1,command.find(")",0)-1));
			if(itr==SymbolTable.end()) //means key not available
			{
				SymbolTable[command.substr(1,command.find(")",0)-1)]=counter;
				counter++;
			}
			else //key is available*/
			SymbolTable[command.substr(1, command.find(")", 0) - 1)] = counter;
			symbols.push_back(command.substr(1, command.find(")", 0) - 1));
			//counter++;
			continue;
		}
		if (command.substr(0, 1) == "@")
		{
			if (command[1] - '0' > 9)
			{
				//std::map::iterator itr;
				auto itr = SymbolTable.find(command.substr(1, command.find(" ", 0) - 1));
				if (itr == SymbolTable.end()) //means key not available
				{
					SymbolTable[command.substr(1, command.find(" ", 0) - 1)] = -1;
					symbols.push_back(command.substr(1, command.find(")", 0) - 1));
				}
				//else key is available then we will update in the statements later during the second pass
			}
		}
		for (i = 0; i < command.length(); i++)
		{
			if (command.substr(i, 1) == " " || command.substr(i, 1) == "\t") continue;
			if (command.substr(i, 2) == "//") break;//ignore the comment
			newcomm += command.substr(i, 1);
		}
		counter++;
		fint << newcomm << endl;
	}
	for (i=0; i < symbols.size(); i++)
	{
		if (SymbolTable[symbols[i]] != -1) continue;
		SymbolTable[symbols[i]] = newkey;
		newkey++;
	}
	//Symbol Table created
	fint.close();
	myfile.close();

	ifstream finter(intfile.c_str());
	//fint.open(intfile.c_str());
	string hackcommand;//we will take normal command from int file and output hack command
	while (getline(finter, command))
	{
		//Now there are no comments or spaces
		hackcommand = "";
		//A-instruction
		if (command.substr(0, 1) == "@")
		{
			int temp;
			if (command[1] - '0' > 9)
			{
				temp = SymbolTable[command.substr(1, command.find(" ", 0) - 1)];
			}
			else
			{
				temp = stoi(command.substr(1, command.find(" ", 0) - 1));
			}
			hackcommand = "0";
			to_binary(temp, hackcommand);
			fhack << hackcommand << endl;
			continue;
		}
		//else C-instruction
		hackcommand = "111";
		convert_toC(command, hackcommand);
		fhack << hackcommand << endl;
	}
	finter.close();
	fhack.close();
}

