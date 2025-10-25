#ifndef ERROR_H
#define ERROR_H

#include <iostream>
#include <exception>

#include "../Parser/parser.h"
#include "../Parser/Lexer/lexer.h"

// Class for Lexical errors
class LexicalError: public std::exception {
private:
	std::string message_;

public:
	LexicalError(Lexer& lexer) {
		this->message_ = "LexicalError(" + lexer.file_name + "): " + lexer.stream.code + "\nUnknown symbol (" 
			+ lexer.stream.current_char
			+ ") in " + std::to_string(lexer.stream.line) + ":" + std::to_string(lexer.stream.column);
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
		this->message_ = "Synatx error occured\n";
	}
	const char* what() const throw() {
		return this->message_.c_str();
	}
};
#endif // !ERROR_H