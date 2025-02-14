#ifndef LEXER_H
#define LEXER_H

#include <cctype>
#include <map>
#include <vector>

#include "CharStream/CharStream.h"
#include "../Tokens/tokens.h"

enum LexerState { LINE = 0, IN_BLOCK };

struct Token {
	TokenType type;
	std::string value;
	size_t line, column;
	Token(size_t line, size_t column, TokenType type, const std::string& value) : line(line), column(column), type(type), value(value) {}
};

class Lexer{
public:
	std::string reserved_words[12] = { "var", "const", "int", "string", "char", "bool", "float", "while", "for", "if", "else", "func" };
	std::map<std::string, TokenType> reserved_word_types = {
		{"var", TokenType::VAR_KEYWORD}, {"const", TokenType::CONST_KEYWORD}, {"while", TokenType::WHILE_KEYWORD}, {"for", TokenType::FOR_KEYWORD}, 
		{"foreach", TokenType::FOREACH_KEYWORD}, {"if", TokenType::IF_KEYWORD}, {"else", TokenType::ELSE_KEYWORD}, {"func", TokenType::FUNC_KEYWORD}, 
		{"int", TokenType::VARIABLE_TYPE}, {"float", TokenType::VARIABLE_TYPE}, {"char", TokenType::VARIABLE_TYPE}, {"bool", TokenType::VARIABLE_TYPE},
		{"string", TokenType::VARIABLE_TYPE}
	};
	std::map<char, TokenType> reserved_symbols = {
		{';', TokenType::SEMICOLON}, {':', TokenType::COLON}, {'=', TokenType::EQUAL}, {'.', TokenType::DOT}, {'"', TokenType::QUOTE},
		{'<', TokenType::LESS}, {'>', TokenType::GREATER}, {'+', TokenType::PLUS}, {'-', TokenType::MINUS}, {'*', TokenType::MULTIPLY}, {'/', TokenType::DIVIDE},
		{'!', TokenType::NOT}, {'(', TokenType::LRPAREN}, {')', TokenType::RRPAREN}, {'{', TokenType::LFPAREN}, {'}', TokenType::RFPAREN},
		{'|', TokenType::UNARY_LOGIC_OR}, {'&', TokenType::UNARY_LOGIC_AND}
	};
	std::map<std::string, TokenType> resreved_binary_operation_ = {
		{">=", TokenType::GREATER_EQUAL}, {"<=", TokenType::LESS_EQUAL}, {"==", TokenType::EQUAL_EQUAL}, {"!=", TokenType::NOT_EQUAL},
		{"||", TokenType::LOGIC_OR}, {"&&", TokenType::LOGIC_AND}, {"+=", TokenType::PLUS_EQUAL}, {"-=", TokenType::MINUS_EQUAL}, 
		{"*=", TokenType::MULTIPLY_EQUAL}, {"/=", TokenType::DIVIDE_EQUAL}
	};

private:
	LexerState state_ = LexerState::LINE;
	CharStream stream_;
	std::vector<Token*> token_list_;

private:
	Token* getId(); // Lex variable name or command
	Token* Lex(); // Lex tokens
	Token* getNum(); // Lex nums
	Token* getString(); // Lex string
	Token* getSymbol(); // Lex symbols
	void getBlockOfCode(); // get block of code from { }
	bool isReservedKey(const std::string& key); // Is key in resreved words?
	bool isReservedSymbol(char symbol); // Is symbol in resreved symbols

public:
	Lexer() = default;
	void clear(){ token_list_.clear(); } // Clear tokens list 
	void initLexer(const char* code, const char* file="<stdin>"); // Initialize the lexer
	CharStream returnStream(); // Return stream for LexicalError class
	std::vector<Token*> getListOfTokens();
};
#endif // !LEXER_H