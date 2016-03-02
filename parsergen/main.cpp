#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

struct data {
	std::string mType;
	std::string mName;
};

void OutputSwitch(std::vector <std::string> &dataVec, std::size_t charIndex, std::ofstream &file, std::map <std::string, std::string> &types, std::map <std::string, struct data> &datas)
{
	bool switchAdded = false;
	if (dataVec.size() == 1)
	{ 
		std::string toReplace = "{DESTINATIONLOC}";
		struct data dataVal = datas[dataVec[0]];
		std::string typeString = types[dataVal.mType];
		std::size_t pos = typeString.find(toReplace);
		typeString.replace(pos, toReplace.length(), dataVal.mName);
		file << typeString << std::endl;
	}
	else
	{
		std::vector <std::string> newDataVec;
		char currentChar = ' ';
		for (std::size_t i = 0; i < dataVec.size(); i++)
		{
			char newCurrentChar = dataVec[i][charIndex];
			if (newCurrentChar != currentChar && currentChar != ' ')
			{
				if (!switchAdded)
				{
					file << "switch(line[" << charIndex+1 << "]) {" << std::endl;
					switchAdded = true;
				}
				file << "case '" << currentChar << "':{" << std::endl;
				OutputSwitch(newDataVec, charIndex + 1, file, types, datas);
				file << "break;" << std::endl << "}" << std::endl;
				currentChar = newCurrentChar;
				newDataVec.clear();
			}
			else if (currentChar == ' ')
			{
				currentChar = newCurrentChar;
			}
			newDataVec.push_back(dataVec[i]);
		}
		if (newDataVec.size() != 0)
		{
			if (newDataVec.size() != dataVec.size()) 
			{ 
				file << "case '" << currentChar << "':{" << std::endl; 
			}

			OutputSwitch(newDataVec, charIndex + 1, file, types, datas);

			if (newDataVec.size() != dataVec.size()) 
			{
				file << "break;" << std::endl << "}" << std::endl;
			}
		}
	}
	if (switchAdded)
	{
		file << "}" << std::endl;
	}
}

int main(int argc, char *argv[])
{
	std::map <std::string, std::string> types;
	std::map <std::string, struct data> datas;
	std::vector <std::string> dataVec;
	std::string structType;
	char * parserDefPath = argv[1];

	std::cout << "Loading: " << parserDefPath << std::endl;
	
	std::string line;
	std::ifstream myfile(parserDefPath);
	std::string customHeaderString = "";
	if (myfile.is_open())
	{
		bool customHeader = false;
		while (getline(myfile, line))
		{
			if (line.compare(0, 4, "TYPE") == 0 && !customHeader)
			{
				std::size_t pos1Space, pos2Space;
				pos1Space = line.find(' ')+1;
				pos2Space = line.find(' ', pos1Space);
				std::string key = line.substr(pos1Space, pos2Space - pos1Space);
				std::string val = line.substr(pos2Space + 1);
				types[key] = val;
			}
			else if (line.compare(0, 6, "STRUCT") == 0 && !customHeader)
			{
				std::size_t pos1Space = line.find(' ') + 1;
				structType = line.substr(pos1Space);
			}
			else if (line.compare(0, 4, "DATA") == 0 && !customHeader)
			{
				std::size_t pos1Space, pos2Space, pos3Space;
				pos1Space = line.find(' ') + 1;
				pos2Space = line.find(' ', pos1Space) + 1;
				pos3Space = line.find(' ', pos2Space) + 1;
				std::string key = line.substr(pos1Space, pos2Space - pos1Space - 1);
				struct data dataInst;
				dataInst.mType = line.substr(pos2Space, pos3Space - pos2Space - 1);
				dataInst.mName = line.substr(pos3Space);
				datas[key] = dataInst;
				dataVec.push_back(key);
			}
			else if (line.compare(0, 17, "STARTCUSTOMHEADER") == 0)
			{
				customHeader = true;
			}
			else if (line.compare(0, 15, "ENDCUSTOMHEADER") == 0)
			{
				customHeader = false;
			}
			else if (customHeader)
			{
				customHeaderString += line;
			}
		}
		myfile.close();
	}
	else 
	{
		std::cout << "Unable to open file" << std::endl;
	}

	std::ofstream ouputHeaderFile(structType + "Parser.h");
	ouputHeaderFile << "#ifndef __" << structType << "__H__" << std::endl;
	ouputHeaderFile << "#define __" << structType << "__H__" << std::endl;
	if (customHeaderString.length() != 0)
	{
		ouputHeaderFile << std::endl;
		ouputHeaderFile << "//CUSTOM HEADER START" << std::endl;
		ouputHeaderFile << customHeaderString << std::endl;
		ouputHeaderFile << "//CUSTOM HEADER END" << std::endl;
		ouputHeaderFile << std::endl;
	}
	ouputHeaderFile << "void Load" << structType << "(char * path, " << structType << " *def);" << std::endl;
	ouputHeaderFile << "#endif" << std::endl;
	ouputHeaderFile.close();

	std::ofstream ouputcppFile(structType + "Parser.cpp");
	ouputcppFile << "#include \"" << structType + "Parser.h" << "\"" << std::endl;
	ouputcppFile << "#include <stdio.h>" << std::endl;
	ouputcppFile << "#include <string.h>" << std::endl;
	ouputcppFile << std::endl;
	ouputcppFile << "void " << structType << "ParseLine(char * line, " << structType << " *def) {" << std::endl;
	std::sort(dataVec.begin(), dataVec.end());
	OutputSwitch(dataVec, 0, ouputcppFile, types, datas);
	ouputcppFile << "}" << std::endl;
	ouputcppFile << std::endl;
	ouputcppFile << "void Load" << structType << "(char * path, " << structType << " *def) {" << std::endl;
	ouputcppFile << "FILE * file = fopen(path, \"rt\");" << std::endl;
	ouputcppFile << "char line[120];" << std::endl;
	ouputcppFile << "while(fgets(line, 120, file) != NULL) {" << std::endl;
	ouputcppFile << structType << "ParseLine(line, def);" << std::endl;
	ouputcppFile << "}" << std::endl << "}" << std::endl;
	ouputcppFile.close();

    return 0;
}
