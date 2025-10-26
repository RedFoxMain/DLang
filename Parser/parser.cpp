#include "parser.h"
#include "../Error/error.h"

// Main function
std::vector<AST*> Parser::parse(std::vector<Token*> token_list) {
	tokens_ = std::move(token_list);
	current_token_pos_ = 0; // Set current pos of token in tokens list
	m_current_token = tokens_[current_token_pos_]; // Set first token
	return parseStatement();
}

std::vector<AST*> Parser::parseStatement(bool if_block) {
	std::vector<AST*> ast;
	while (m_current_token->type != TokenType::END_OF_FILE) {
		if (if_block && m_current_token->type == RFPAREN) break;
		if (m_current_token->value == "var" || m_current_token->value == "const") { ast.push_back(parseVarDeclaration()); }
		if (match(TokenType::ID)) { ast.push_back(parseVarReasign()); }
		if (m_current_token->value == "if") { consume(TokenType::IF_KEYWORD);  ast.push_back(parseIf()); }
		if (m_current_token->value == "while") { consume(TokenType::WHILE_KEYWORD);  ast.push_back(parseWhile()); }
	}
	return ast;
}

// Check current token
bool Parser::match(TokenType type) {
	if (m_current_token->type != type) { return false; }
	return true;
}

// Get current token
bool Parser::isEndOfFile() {
	return m_current_token->type == TokenType::END_OF_FILE;
}

// Apply current token
void Parser::consume(TokenType type) {
	if (isEndOfFile() || !match(type)) {
		raiseError(std::format("SYNTAX ERROR: Unexpected Token {} in line: {}\n", m_current_token->value, m_current_token->line));
	}
	m_current_token = advance();
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
		ast = new IntNode(m_current_token); consume(TokenType::INT);
		return ast; 
	}
	if (match(TokenType::STRING)) { 
		ast = new StrNode(m_current_token); consume(TokenType::STRING);
		return ast; 
	}
	if (match(TokenType::FLOAT)) { 
		ast = new FloatNode(m_current_token); consume(TokenType::FLOAT);
		return ast; 
	}
	if (match(TokenType::LRPAREN)) { 
		consume(TokenType::LRPAREN); ast = expr(); consume(TokenType::RRPAREN); 
		return ast; 
	}
	if (match(TokenType::LSPAREN)) return parseArray();
	if (match(TokenType::ID)) return parseVariable();
	if (match(TokenType::MINUS)) {  
		Token* token = m_current_token; 
		consume(TokenType::MINUS); ast = new UnOpNode(token, factor());
		return ast; 
	}
	if (match(TokenType::PLUS)) { 
		Token* token = m_current_token; 
		consume(TokenType::PLUS); ast = new UnOpNode(token, factor());
		return ast;
	}
	if (match(TokenType::INCREMENT)) {
		Token* token = m_current_token;
		consume(TokenType::INCREMENT); ast = new UnOpNode(token, factor());
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
		Token* token = m_current_token;
		if (match(TokenType::MULTIPLY)) { consume(TokenType::MULTIPLY); }
		if (match(TokenType::DIVIDE)) { consume(TokenType::DIVIDE); }

		if (match(TokenType::LESS)) { consume(TokenType::LESS); }
		if (match(TokenType::GREATER)) { consume(TokenType::GREATER); }

		if (match(TokenType::GREATER_EQUAL)) { consume(TokenType::GREATER_EQUAL); }
		if (match(TokenType::LESS_EQUAL)) { consume(TokenType::LESS_EQUAL); }
		if (match(TokenType::NOT_EQUAL)) { consume(TokenType::NOT_EQUAL); }
		if (match(TokenType::EQUAL_EQUAL)) { consume(TokenType::EQUAL_EQUAL); }
		ast = new BinOpNode(ast, token, factor());
	}
	return ast;
}

AST* Parser::expr() {
	AST* ast = term();
	while (match(TokenType::PLUS) || match(TokenType::MINUS) ||
		match(TokenType::LOGIC_AND) || match(TokenType::LOGIC_OR)) {
		Token* token = m_current_token;
		if (match(TokenType::PLUS)) { consume(TokenType::PLUS); }
		if (match(TokenType::MINUS)) { consume(TokenType::MINUS); }

		if (match(TokenType::LOGIC_AND)) { consume(TokenType::LOGIC_AND); }
		if (match(TokenType::LOGIC_OR)) { consume(TokenType::LOGIC_OR); }
		ast = new BinOpNode(ast, token, term());
	}
	return ast;
}

// Parse variable names
IdNode* Parser::parseVariable() {
	IdNode* ast = nullptr;
	if (match(TokenType::ID)) {
		ast = new IdNode(m_current_token);
		consume(TokenType::ID);
	}
	return ast;
}

// Parse variable declaration
AST* Parser::parseVarDeclaration() {
	AST* ast = nullptr;
	
	// Get var or const keyword
	Token* key_word = m_current_token;
	switch (key_word->type) {
		case TokenType::VAR_KEYWORD:
		case TokenType::CONST_KEYWORD:
			consume(m_current_token->type);
	}
	
	// Get vriable name
	IdNode* id = parseVariable();
	
	// Get type assign operator
	Token* var_assign_op = m_current_token;
	consume(TokenType::COLON);
	
	// Get variable type
	Token* var_type = m_current_token;
	consume(TokenType::VARIABLE_TYPE);
	
	// If current token is SEMICOLON create empty variable declaration node
	if (match(TokenType::SEMICOLON)) { 
		ast = new EmptyVarDeclNode(key_word, id, var_assign_op, var_type);
	}else { // Else create full variable declaration node
		Token* assign = m_current_token;
		consume(TokenType::EQUAL);
		ast = new FullVarDeclNode(new EmptyVarDeclNode(key_word, id, var_assign_op, var_type), assign, expr());
	}
	consume(TokenType::SEMICOLON);
	return ast;
}

// Parse var reasigment
ReasignVarNode* Parser::parseVarReasign() {
	IdNode* id = parseVariable(); // Get variable name
	
	// Get expression assign operator
	Token* assign = m_current_token;
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
	return new ReasignVarNode(id, assign, expression);
}

// Parse list of code
BlockOfCodeNode* Parser::parseListOfCode() {
	consume(TokenType::LFPAREN);
	std::vector<AST*> list = parseStatement(true);
	consume(TokenType::RFPAREN);
	return new BlockOfCodeNode(list);
}

ArrayNode* Parser::parseArray() {
	consume(TokenType::LSPAREN);
	std::vector<Token*> temp;
	while (m_current_token) {
		temp.push_back(m_current_token);
		m_current_token = advance();
		if (match(TokenType::RSPAREN)) {
			consume(TokenType::RSPAREN);
			break;
		}
		consume(TokenType::COMMA);
	}
	return new ArrayNode(temp);
}

// Parse if statement
IfStmtNode* Parser::parseIf() {
	consume(TokenType::LRPAREN);
	AST* condition = expr();
	consume(TokenType::RRPAREN);
	return new IfStmtNode(condition, parseListOfCode());
}

WhileStmtNode* Parser::parseWhile() {
	consume(TokenType::LRPAREN);
	AST* condition = expr();
	consume(TokenType::RRPAREN);
	return new WhileStmtNode(condition, parseListOfCode());
}