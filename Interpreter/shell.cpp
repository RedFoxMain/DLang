#include <iostream>
#include <fstream>
#include "interpreter.h"
#include "../Error/error.h"

std::string readFromFile(const std::string& file_name) {
	std::string temp;
	std::ifstream file(file_name);
	if (!file.is_open()) std::cerr << "Cant't find file: " << file_name << std::endl;
	std::string line;
	while (std::getline(file, line))
		temp += line + "\n";
	return temp;
}

int main() {
	Lexer lexer;
	Parser parser;
	ASTPrinter printer;

	try {
		std::vector<Token*> tokens = lexer.lex(readFromFile("file_path"));
		std::vector<AST*> data = parser.parse(tokens);
		for (AST* ast : data) if (ast) printer.print(ast);
	}
	catch (std::exception& err) {
		std::cout << err.what();
	}
	return 0;
}