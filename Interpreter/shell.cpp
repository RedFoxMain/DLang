#include <iostream>
#include "Interpreter.h"

int main() {
	Lexer lexer;
	Parser parser;
	ASTPrinter printer;
	
	for (;;) {
		std::string line;
		std::cout << ">> ";  std::getline(std::cin, line);
		Interpreter::setEnv(lexer, parser);
		Interpreter::exec(line, "<stdin>", &printer);
	}
	return 0;
}