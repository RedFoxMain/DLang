#ifndef LEXER_H
#define LEXER_H

#include <cctype>
#include <map>
#include <vector>

#include "CharStream/char_stream.h"
#include "../Tokens/tokens.h"

struct Token {
	TokenType type;
	std::string value;
	size_t line, column;
	Token(size_t line, size_t column, TokenType type, const std::string& value) 
		: line(line), column(column), type(type), value(value) {}
};

class Lexer{
public:
	CharStream stream;
	std::string file_name;

	std::vector<std::string> reserved_words = { "var", "const", "int", "string", "char", "bool", "float", "void", "while", "for", "if", "else", "func" };
	std::map<std::string, TokenType> reserved_word_types = {
		{"var", TokenType::VAR_KEYWORD}, {"const", TokenType::CONST_KEYWORD}, {"while", TokenType::WHILE_KEYWORD}, {"for", TokenType::FOR_KEYWORD}, 
		{"foreach", TokenType::FOREACH_KEYWORD}, {"if", TokenType::IF_KEYWORD}, {"else", TokenType::ELSE_KEYWORD}, {"func", TokenType::FUNC_KEYWORD}, 
		{"int", TokenType::VARIABLE_TYPE}, {"float", TokenType::VARIABLE_TYPE}, {"char", TokenType::VARIABLE_TYPE}, {"bool", TokenType::VARIABLE_TYPE},
		{"string", TokenType::VARIABLE_TYPE}, {"array", TokenType::VARIABLE_TYPE}, {"void", TokenType::VARIABLE_TYPE}
	};
	std::map<char, TokenType> reserved_symbols = {
		{';', TokenType::SEMICOLON}, {':', TokenType::COLON}, {'=', TokenType::EQUAL}, {'.', TokenType::DOT}, {'"', TokenType::QUOTE}, {',', TokenType::COMMA},
		{'<', TokenType::LESS}, {'>', TokenType::GREATER}, {'+', TokenType::PLUS}, {'-', TokenType::MINUS}, {'*', TokenType::MULTIPLY}, {'/', TokenType::DIVIDE},
		{'!', TokenType::NOT}, {'(', TokenType::LRPAREN}, {')', TokenType::RRPAREN}, {'{', TokenType::LFPAREN}, {'}', TokenType::RFPAREN},
		{'[', TokenType::LSPAREN}, {']', TokenType::RSPAREN}, {'|', TokenType::UNARY_LOGIC_OR}, {'&', TokenType::UNARY_LOGIC_AND}
	};
	std::map<std::string, TokenType> resreved_binary_operation_ = {
		{">=", TokenType::GREATER_EQUAL}, {"<=", TokenType::LESS_EQUAL}, {"==", TokenType::EQUAL_EQUAL}, {"!=", TokenType::NOT_EQUAL},
		{"||", TokenType::LOGIC_OR}, {"&&", TokenType::LOGIC_AND}, {"+=", TokenType::PLUS_EQUAL}, {"-=", TokenType::MINUS_EQUAL}, 
		{"*=", TokenType::MULTIPLY_EQUAL}, {"/=", TokenType::DIVIDE_EQUAL}, {"++", TokenType::INCREMENT}, {"--", TokenType::DECREMENT},
		{"->", TokenType::ANNOTATION}
	};

private:
	std::vector<Token*> m_tokens;

private:
	Token* getId(); // Lex variable name or command
	Token* getNum(); // Lex nums
	Token* getString(); // Lex string
	Token* getSymbol(); // Lex symbols
	bool isReservedKey(const std::string& key); // Is key in resreved words?
	bool isReservedSymbol(char symbol); // Is symbol in resreved symbols
	Token* getToken();

public:
	Lexer() = default;
	std::vector<Token*> lex(const std::string& code, const char* file = "<stdin>"); // get list of tokens
};
#endif // !LEXER_H