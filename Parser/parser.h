#ifndef PARSER_H
#define PARSER_H

#include "AST/ast.h"

class Parser {
private:
	int current_token_pos_ = 0;
	bool error_occured_ = false;
	std::vector<Token*> tokens_;
	Token* current_token_;

private:
	bool match(TokenType type); // Match current token
	bool isEndOfFile(); // Is current type of token equal EOF
	void consume(TokenType type); // Apply current token
	Token* advance(); // Get token
	
	// Functions for parsing expressions
	AST* factor();
	AST* term();
	AST* expr();

	Id* parseVariable(); // Parse variable names
	AST* parseVarDeclaration(); // Parse variable declaration
	ReasignVar* parseVarReasign(); // Parse var reasigment
	If* parseIf(); // Parse if statement
	BlockOfCode* parseListOfCode(); // Parse list of code
	std::vector<AST*> parseStatement(bool if_block = false); // Main function

public:
	Parser() = default;
	std::vector<AST*> parse(std::vector<Token*> token_list);
	Token* returnCurrToken() { return current_token_; }
	Token* getPrevious(); // Get previous token
};
#endif // !PARSER_H