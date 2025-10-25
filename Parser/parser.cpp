#include "parser.h"
#include "../Error/error.h"

// Main function
std::vector<AST*> Parser::parse(std::vector<Token*> token_list) {
	tokens_ = std::move(token_list);
	current_token_pos_ = 0; // Set current pos of token in tokens list
	current_token_ = tokens_[current_token_pos_]; // Set first token
	return parseStatement();
}

std::vector<AST*> Parser::parseStatement(bool if_block) {
	std::vector<AST*> ast;
	while (current_token_->type != TokenType::END_OF_FILE) {
		if (if_block && current_token_->type == RFPAREN) break;
		if (current_token_->value == "var" || current_token_->value == "const") { ast.push_back(parseVarDeclaration()); }
		if (match(TokenType::ID)) { ast.push_back(parseVarReasign()); }
		if (current_token_->value == "if") { consume(TokenType::IF_KEYWORD);  ast.push_back(parseIf()); }
	}
	return ast;
}

// Check current token
bool Parser::match(TokenType type) {
	if (current_token_->type != type) { return false; }
	return true;
}

// Get current token
bool Parser::isEndOfFile() {
	return current_token_->type == TokenType::END_OF_FILE;
}

// Apply current token
void Parser::consume(TokenType type) {
	if (isEndOfFile() || !match(type)) { throw SyntaxError(*this);  }
	current_token_ = advance();
}

// Return previous token
Token* Parser::getPrevious() {
	return tokens_[(current_token_pos_ > 0) ? current_token_pos_ - 1 : 0 ];
}

// Get next token
Token* Parser::advance() {
	return (current_token_pos_ < tokens_.size()) ? tokens_[++current_token_pos_] : nullptr;
}

// For math
AST* Parser::factor() {
	AST* ast = nullptr;
	if (match(TokenType::INT)) { 
		ast = new Int(current_token_); consume(TokenType::INT);  
		return ast; 
	}
	if (match(TokenType::STRING)) { 
		ast = new Str(current_token_); consume(TokenType::STRING); 
		return ast; 
	}
	if (match(TokenType::FLOAT)) { 
		ast = new Float(current_token_); consume(TokenType::FLOAT); 
		return ast; 
	}
	if (match(TokenType::LRPAREN)) { 
		consume(TokenType::LRPAREN); ast = expr(); consume(TokenType::RRPAREN); 
		return ast; 
	}
	if (match(TokenType::ID)) { return parseVariable(); }
	if (match(TokenType::MINUS)) {  
		Token* token = current_token_; 
		consume(TokenType::MINUS); ast = new UnOp(token, factor()); 
		return ast; 
	}
	if (match(TokenType::PLUS)) { 
		Token* token = current_token_; 
		consume(TokenType::PLUS); ast = new UnOp(token, factor()); 
		return ast;
	}
	return ast;
}

AST* Parser::term() {
	AST* ast = factor();
	while (match(TokenType::MULTIPLY) || match(TokenType::DIVIDE) ||
		match(TokenType::GREATER_EQUAL) || match(TokenType::LESS_EQUAL) ||
		match(TokenType::EQUAL_EQUAL) || match(TokenType::NOT_EQUAL) ||
		match(TokenType::LESS) || match(TokenType::GREATER)) {
		Token* token = current_token_;
		if (match(TokenType::MULTIPLY)) { consume(TokenType::MULTIPLY); }
		if (match(TokenType::DIVIDE)) { consume(TokenType::DIVIDE); }

		if (match(TokenType::LESS)) { consume(TokenType::LESS); }
		if (match(TokenType::GREATER)) { consume(TokenType::GREATER); }

		if (match(TokenType::GREATER_EQUAL)) { consume(TokenType::GREATER_EQUAL); }
		if (match(TokenType::LESS_EQUAL)) { consume(TokenType::LESS_EQUAL); }
		if (match(TokenType::NOT_EQUAL)) { consume(TokenType::NOT_EQUAL); }
		if (match(TokenType::EQUAL_EQUAL)) { consume(TokenType::EQUAL_EQUAL); }
		ast = new BinOp(ast, token, factor());
	}
	return ast;
}

AST* Parser::expr() {
	AST* ast = term();
	while (match(TokenType::PLUS) || match(TokenType::MINUS) ||
		match(TokenType::LOGIC_AND) || match(TokenType::LOGIC_OR)) {
		Token* token = current_token_;
		if (match(TokenType::PLUS)) { consume(TokenType::PLUS); }
		if (match(TokenType::MINUS)) { consume(TokenType::MINUS); }

		if (match(TokenType::LOGIC_AND)) { consume(TokenType::LOGIC_AND); }
		if (match(TokenType::LOGIC_OR)) { consume(TokenType::LOGIC_OR); }
		ast = new BinOp(ast, token, term());
	}
	return ast;
}

// Parse variable names
Id* Parser::parseVariable() {
	Id* ast = nullptr;
	if (match(TokenType::ID)) {
		ast = new Id(current_token_);
		consume(TokenType::ID);
	}
	return ast;
}

// Parse variable declaration
AST* Parser::parseVarDeclaration() {
	AST* ast = nullptr;
	
	// Get var or const keyword
	Token* key_word = current_token_;
	switch (key_word->type) {
		case TokenType::VAR_KEYWORD:
		case TokenType::CONST_KEYWORD:
			consume(current_token_->type);
	}
	
	// Get vriable name
	Id* id = parseVariable();
	
	// Get type assign operator
	Token* var_assign_op = current_token_;
	consume(TokenType::COLON);
	
	// Get variable type
	Token* var_type = current_token_;
	consume(TokenType::VARIABLE_TYPE);
	
	// If current token is SEMICOLON create empty variable declaration node
	if (match(TokenType::SEMICOLON)) { 
		ast = new EmptyVarDecl(key_word, id, var_assign_op, var_type); 
	}else { // Else create full variable declaration node
		Token* assign = current_token_;
		consume(TokenType::EQUAL);
		ast = new FullVarDecl(new EmptyVarDecl(key_word, id, var_assign_op, var_type), assign, expr());
	}
	consume(TokenType::SEMICOLON);
	return ast;
}

// Parse var reasigment
ReasignVar* Parser::parseVarReasign() {
	Id* id = parseVariable(); // Get variable name
	
	// Get expression assign operator
	Token* assign = current_token_;
	switch (assign->type) {
		case TokenType::EQUAL:
		case TokenType::PLUS_EQUAL:
		case TokenType::MINUS_EQUAL:
		case TokenType::MULTIPLY_EQUAL:
		case TokenType::DIVIDE_EQUAL:
			consume(assign->type);
	}
	AST* expression = expr();
	consume(TokenType::SEMICOLON);
	return new ReasignVar(id, assign, expression);
}

// Parse list of code
BlockOfCode* Parser::parseListOfCode() {
	consume(TokenType::LFPAREN);
	std::vector<AST*> list = parseStatement(true);
	consume(TokenType::RFPAREN);
	return new BlockOfCode(list);
}

// Parse if statement
If* Parser::parseIf() {
	consume(TokenType::LRPAREN);
	AST* condition = expr();
	consume(TokenType::RRPAREN);
	return new If(condition, parseListOfCode());
}