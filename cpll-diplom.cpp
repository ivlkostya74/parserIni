// cpll-diplom.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <iomanip>
#include <windows.h>
#include "Parser.h"


int main()
{
	setlocale(LC_ALL, "Russian");
	try
	{
		
		auto print_lambda = [](Parser& par, const parseVar_t& var)
		{
			std::cout << "  <"
				<< par.getSection() << "."
				<< par.getVarName() << "="
				<< var << ">\n";
		};

		Parser parser("FileName.ini");

		auto varValue = parser.get_value<double>("Section1.var1");
		print_lambda(parser, varValue);

		varValue = parser.get_value<int>("Section2.var1");
		print_lambda(parser, varValue);

		varValue = parser.get_value<std::string>("Section2.var2");
		print_lambda(parser, varValue);

		varValue = parser.get_value<double>("Section1.var2");
		print_lambda(parser, varValue);
	}
	catch (const std::exception& err)
	{

		std::cout << "\nОшибка типа: " << typeid(err).name() << "\n";
		std::cout << err.what() << "\n";

	}

	return 0;
}



// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
