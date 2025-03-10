#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "../Parser/AST/AstPrinter.h"
#include "../Parser/Lexer/Lexer.h"
#include "../Parser/Parser.h"

static Lexer lexer;
static Parser parser;

class Interpreter {
public:
	Interpreter() = default;

	// Set lexer, parser
	static void setEnv(Lexer& lexer, Parser& parser) {
		lexer = lexer;
		parser = parser;
	}

	// Execute code from text
	static void exec(const std::string& code, const char* file_name = "<stdin>", ASTPrinter* printer = nullptr) {
		lexer.initLexer(code, file_name);
		try {
			for (Token* tk : lexer.getListOfTokens()) {
				if (tk) { std::cout << tk->value << std::endl; }
			}
			//AST* ast = parser.parse(lexer.getListOfTokens());
			//if (printer) { printer->print(ast); }
		} catch (const std::exception& e) { std::cout << e.what() << "\n"; }
		lexer.clear();
	}
};
#endif // !INTERPRETER_H