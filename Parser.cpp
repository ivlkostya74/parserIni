#include "Parser.h"
#include <iostream>

void Parser::deleteSpace(std::string& str)
{
	str.erase(std::remove_if(str.begin(), str.end(), isspace), str.end());
}

void Parser::printBuf(std::string_view s)
{
	
	std::cout << "\n" << s << "\n";
	for (const auto& val : buf) std::cout << val << "\n";
	
}


void Parser::someCleanBuf()
{
	for (auto it = buf.begin(); it != buf.end(); )
	{
		std::string line = *it;
	
		line.erase(std::find(line.begin(), line.end(), ';'), line.end());
		
		deleteSpace(line);

		if (line.find(varName) != std::string::npos) ++it;
		else it = buf.erase(it);
	}
}

Parser::Parser(std::string _fileName) : fileName(std::move(_fileName))
{
}


std::string Parser::getSection()
{
	return secName;
}


std::string Parser::getVarName()
{
	return varName;
}

std::ostream& operator<<(std::ostream& out, const parseVar_t& var)
{
	std::visit([&out](auto&& arg) { out << arg; }, var);
	return out;
}
