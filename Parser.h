#include <fstream>
#include <string>
#include <algorithm>
#include <charconv>
#include <vector>
#include <variant>

using parseVar_t = std::variant<int, double, std::string>;


std::ostream& operator<< (std::ostream& out, const parseVar_t& var);

class Parser
{
protected:
private:
	std::string fileName;			// ��� �����
	std::string varName, secName;	// ������� ���������� � ������
	std::vector<std::string> buf;

	void deleteSpace(std::string& str);

	// �������������� ������ � ����� ���� <T>
	template <typename T>
	T fromChar(std::string s);
	// ������ ������ � �������
	void printBuf(std::string_view s);
	// ������ ������, �������� ������ ���������� ������� ����������
	void someCleanBuf();

public:
	// ��������� ��� ����� � ���� ������
	Parser(std::string _fileName);

	template <typename T>
	parseVar_t get_value(std::string _findStr);

	std::string getSection();
	std::string getVarName();
};



// �������������� ������ � ����� ���� <T>
template<typename T>
inline T Parser::fromChar(std::string s)
{
    T value{};
	auto [ptr, ec] { std::from_chars(s.data(), s.data() + s.size(), value) };
	if (ec == std::errc()) return value;
	else if (ec == std::errc::invalid_argument)
	{
		someCleanBuf(); // ������ ������, �������� ������ ���������� ������� ����������
		printBuf("������� �������� ����������...");
		throw std::runtime_error("���������� <" + secName + '.' + varName + "> �� �������� �����!");
	}
	else if (ec == std::errc::result_out_of_range)
	{
		someCleanBuf(); // ������ ������, �������� ������ ���������� ������� ����������
		printBuf("������� �������� ����������...");
		throw std::out_of_range("�������� ���������� <" + secName + '.' + varName + "> ��������� ��������!");
	}
	else throw std::runtime_error(" ������! ���������� <" + secName + '.' + varName + ">!");
}
 
template <>
inline std::string Parser::fromChar(std::string s)
{
	return std::move(s);
}

 
template<typename T>
inline parseVar_t Parser::get_value(std::string findStr)
{
	secName = std::move(findStr);
	deleteSpace(secName);	// ������ ��������� ������� � ������ ������

	auto dotPos = secName.find('.');
	if (dotPos != std::string::npos)
	{
		varName = secName.substr(dotPos + 1);
		secName = secName.substr(0, dotPos);
	}
	else throw std::runtime_error("��� ������? ��������� ����� ����������� (������.����������)!");
	if (secName.empty())
		throw std::runtime_error("��� ������? ��������� ��� ������ (������.����������)!");
	if (varName.empty())
		throw std::runtime_error("��� ������? ��������� ��� ���������� (������.����������)!");


	bool sectionHaveKey_f(false);	
	bool fileHaveSection_f(false);	
	std::string varValue;			

	std::ifstream file(fileName);
	if (file.is_open())
	{
		unsigned int lineNr{ 0 };
		if (!buf.empty()) buf.clear();
		std::string line;
		bool findVariable_f(false);	

		while (std::getline(file, line))
		{
			++lineNr;	
			if (line.empty()) continue;	


			
			const auto commentPos = line.find_first_of(';');
			  
			const auto openPos = line.find_first_of('[') + 1;
			 
			if (openPos != 0 && openPos <= commentPos)
			{
				 
				const auto closePos = line.find_first_of(']', openPos + 1);

				 
				if (closePos != std::string::npos && closePos < commentPos)
				{
					std::string section = line.substr(openPos, closePos - openPos);//(�������, ���-��)

					if (section == secName)	  
					{
						 
						if (!buf.empty() && !fileHaveSection_f) buf.clear();
						findVariable_f = true;		 
						fileHaveSection_f = true;	 
					}
					else
					{
						findVariable_f = false;		

						if (!fileHaveSection_f) buf.push_back(section);
					}
				}
				else
				{
					throw std::runtime_error("�� ������ ��������� � ������: "
						+ std::to_string(lineNr) + " -> " + line);
				}
				continue;	
			}


			
			if (findVariable_f)
			{
				
				auto str = "������: " + std::to_string(lineNr) + " ->" + line;
				buf.push_back(str); 

				
				line.erase(std::find(line.begin(), line.end(), ';'), line.end());

				
				const auto delimeterPos = line.find('=');
				if (delimeterPos != std::string::npos)
				{
				
					std::string key = line.substr(0, delimeterPos);	
					deleteSpace(key);	
					if (key == varName) 
					{
						sectionHaveKey_f = true;	
						varValue = line.substr(delimeterPos + 1);

						if (varValue.empty()) continue;	
						varValue = varValue.substr(varValue.find_first_not_of(" \t"));
						varValue = varValue.substr(0, varValue.find_last_not_of(" \t") + 1);
					}
				}
				else
				{
					
					line.erase(std::find(line.begin(), line.end(), ';'), line.end());
					deleteSpace(line);	
					if (line.empty())	
					{
						buf.pop_back();	
						continue;		
					}
					throw std::runtime_error("�� ������ ���������. " + buf.back());
				}
			}
		}
		file.close();	
	}
	else throw std::runtime_error("�� ���� ������� ����: " + fileName);

	if (!fileHaveSection_f)
	{
		printBuf("������ �������� ������...");
		throw std::runtime_error("����: " + fileName + " �� �������� ������: " + secName);
	}
	if (!sectionHaveKey_f)
	{
		printBuf("������ ���������� � ������: " + secName);
		throw std::runtime_error("������: " + secName + ", �� �������� ����������: " + varName + "!");
	}
	if (varValue.empty())
	{
		printBuf("������ ���������� � ������: " + secName);
		throw std::runtime_error("���������� " + secName + '.' + varName + " �� ����� ��������!");
	}

	
	return fromChar<T>(std::move(varValue));
}