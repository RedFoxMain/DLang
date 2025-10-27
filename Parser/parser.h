#ifndef PARSER_H
#define PARSER_H

#include "AST/ast.h"

class Parser {
private:
	int current_token_pos_ = 0;
	bool error_occured_ = false;
	std::vector<Token*> tokens_;
	Token* m_current_token;

private:
	bool match(TokenType type); // Match current token
	bool matchNext(TokenType type); // Match next token
	bool isEndOfFile(); // Is current type of token equal EOF
	void consume(TokenType type); // Apply current token
	Token* advance(); // Get token
	Token* getNextToken(); // Get next token
	Token* getPrevToken(); // Get previous token
	
	// Functions for parsing expressions
	AST* factor();
	AST* term();
	AST* expr();

	IdNode* parseId(); // Parse variable names
	AST* parseVarDeclaration(); // Parse variable declaration
	ReasignVarNode* parseVarReasign(); // Parse var reasigment
	IfStmtNode* parseIf(); // Parse if statement
	WhileStmtNode* parseWhile(); // Parse if statement
	BlockOfCodeNode* parseListOfCode(); // Parse list of code
	ArrayNode* parseArray(); // Parse array
	FuncNode* parseFunc(); // Parse function
	FuncParamNode* parseParameters(); // Parse function parameters
	IncDecNode* parseIncDec(); // Parse increment decrement
	std::vector<AST*> parseStatement(bool if_block = false); // Main function

public:
	Parser() = default;
	std::vector<AST*> parse(std::vector<Token*> token_list);
};
#endif // !PARSER_H