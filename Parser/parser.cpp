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
		else if (match(TokenType::ID)) { 
			if(matchNext(TokenType::INCREMENT) || matchNext(TokenType::DECREMENT)) ast.push_back(parseIncDec());
			else ast.push_back(parseVarReasign()); 
		}
		else if (m_current_token->value == "var" || m_current_token->value == "const") { ast.push_back(parseVarDeclaration()); }
		else if (m_current_token->value == "if") { ast.push_back(parseIf()); }
		else if (m_current_token->value == "while") { ast.push_back(parseWhile()); }
		else if (m_current_token->value == "func") { ast.push_back(parseFunc()); }
		else raiseError(std::format("SYNTAX ERROR: Unexpected Token near {} in {}:{}\n", getPrevToken()->value, getPrevToken()->line, getPrevToken()->column));
	}
	return ast;
}

// Check current token
bool Parser::match(TokenType type) {
	if (m_current_token->type != type) { return false; }
	return true;
}

bool Parser::matchNext(TokenType type) {
	if (getNextToken()->type != type) { return false; }
	return true;
}

// Get current token
bool Parser::isEndOfFile() {
	return m_current_token->type == TokenType::END_OF_FILE;
}

// Apply current token
void Parser::consume(TokenType type) {
	if (isEndOfFile() || !match(type)) {
		raiseError(std::format("SYNTAX ERROR: Unexpected Token {} in {}:{}\n", m_current_token->value, m_current_token->line, m_current_token->column));
	}
	m_current_token = advance();
}

// Get next token
Token* Parser::advance() {
	return (current_token_pos_ < tokens_.size()) ? tokens_[++current_token_pos_] : nullptr;
}

Token* Parser::getNextToken() {
	return tokens_[((current_token_pos_ + 1) < tokens_.size() ? current_token_pos_ + 1 : tokens_.size())];
}

Token* Parser::getPrevToken() {
	return tokens_[((current_token_pos_ - 1) > 0 ? current_token_pos_ - 1 : 0)];
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
	if (match(TokenType::ID)) {
		AST* id = parseId();
		if (match(TokenType::INCREMENT)) {
			Token* operation = m_current_token;
			consume(TokenType::INCREMENT); 
			return new UnOpNode(operation, id);
			
		}
		if (match(TokenType::DECREMENT)) {
			Token* operation = m_current_token;
			consume(TokenType::DECREMENT); 
			return new UnOpNode(operation, id);
		}
		return id;
	}
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
	if (match(TokenType::NOT)) {
		Token* token = m_current_token;
		consume(TokenType::NOT); 
		return new UnOpNode(token, factor());
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
IdNode* Parser::parseId() {
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
	IdNode* id = parseId();
	consume(TokenType::COLON);
	
	// Get variable type
	Token* var_type = m_current_token;
	if (var_type->value == "void") 
		raiseError(std::format("SYNTAX ERROR: variable type can't be void {}:{}", m_current_token->line, m_current_token->column));
	consume(TokenType::VARIABLE_TYPE);
	
	// If current token is SEMICOLON create empty variable declaration node
	if (match(TokenType::SEMICOLON)) { 
		ast = new EmptyVarDeclNode(key_word, id, var_type);
		consume(TokenType::SEMICOLON);
		return ast;
	}
	// Else create full variable declaration node
	Token* assign = m_current_token;
	consume(TokenType::EQUAL);
	ast = new FullVarDeclNode(new EmptyVarDeclNode(key_word, id, var_type), assign, expr());
	consume(TokenType::SEMICOLON);
	return ast;
}

// Parse var reasigment
ReasignVarNode* Parser::parseVarReasign() {
	IdNode* id = parseId(); // Get variable name
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

// Parse array
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

// Parse function
FuncNode* Parser::parseFunc() {
	consume(TokenType::FUNC_KEYWORD);
	IdNode* func_name = parseId();
	FuncParamNode* params = parseParameters();
	consume(TokenType::ANNOTATION);
	Token* func_retyrn_type = m_current_token;
	consume(TokenType::VARIABLE_TYPE);
	return new FuncNode(func_name, params, func_retyrn_type, parseListOfCode());
}

// Parse function parametrs
FuncParamNode* Parser::parseParameters() {
	consume(TokenType::LRPAREN);
	std::vector<EmptyVarDeclNode*> params;
	while (m_current_token) {
		Token* key_word = m_current_token;
		switch (key_word->type) {
			case TokenType::VAR_KEYWORD:
			case TokenType::CONST_KEYWORD:
				consume(m_current_token->type);
		}
		IdNode* param_name = parseId();
		consume(TokenType::COLON);
		Token* var_type = m_current_token;
		if (var_type->value == "void") 
			raiseError(std::format("SYNTAX ERROR: function parametr type can't be void {}:{}", m_current_token->line, m_current_token->column));
		consume(TokenType::VARIABLE_TYPE);
		params.push_back(new EmptyVarDeclNode(key_word, param_name, var_type));
		if (match(TokenType::RRPAREN)) break;
		consume(TokenType::COMMA);
	}
	// Parse params
	consume(TokenType::RRPAREN);
	return new FuncParamNode(params);
}

IncDecNode* Parser::parseIncDec() {
	IdNode* id = parseId();
	Token* operation = m_current_token;
	if (match(TokenType::INCREMENT) || match(TokenType::DECREMENT))
		consume(operation->type);
	consume(TokenType::SEMICOLON);
	return new IncDecNode(id, operation);
}

// Parse if statement
IfStmtNode* Parser::parseIf() {
	consume(TokenType::IF_KEYWORD);
	consume(TokenType::LRPAREN);
	AST* condition = expr();
	consume(TokenType::RRPAREN);
	return new IfStmtNode(condition, parseListOfCode());
}

// Parse while statement
WhileStmtNode* Parser::parseWhile() {
	consume(TokenType::WHILE_KEYWORD);
	consume(TokenType::LRPAREN);
	AST* condition = expr();
	consume(TokenType::RRPAREN);
	return new WhileStmtNode(condition, parseListOfCode());
}