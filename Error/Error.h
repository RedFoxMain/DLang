#ifndef ERROR_H
#define ERROR_H

#include <iostream>
#include <exception>

#include "../Parser/Parser.h"
#include "../Parser/Lexer/Lexer.h"

// Class for Lexical errors
class LexicalError: public std::exception {
private:
	std::string message_;

public:
	LexicalError(Lexer& lexer) {
		this->message_ = "LexicalError(" + lexer.returnStream().file + "): " + lexer.returnStream().code + "\nUnknown symbol (" 
			+ lexer.returnStream().current_char 
			+ ") in " + std::to_string(lexer.returnStream().line) + ":" + std::to_string(lexer.returnStream().column);
	}
	const char* what() const throw() {
		return this->message_.c_str();
	}
};

// Class for Syntax errors
class SyntaxError : public std::exception {
private:
	std::string message_;

public:
	SyntaxError(Parser& parser) {
		
	}
	const char* what() const throw() {
		return this->message_.c_str();
	}
};
#endif // !ERROR_H