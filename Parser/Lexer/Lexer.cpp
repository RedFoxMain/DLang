#include "Lexer.h"
#include "../../Error/Error.h"

// Return char stream for LexicalError class
CharStream& Lexer::returnStream() { return stream_; }

// get block of code from { }
void Lexer::getBlockOfCode() {
	state_ = LexerState::LINE;
	int brace_count = 1;
	std::string text;
	Lexer lexer;
	lexer.returnStream().resetLine();
	std::vector<Token*> temp;
	while (brace_count > 0) {
		std::string line;
		std::cout << "...";
		std::getline(std::cin, line);
		if (line.empty()) {
			lexer.returnStream().resetLine();
			continue;
		}
		lexer.initLexer(line, file_name.c_str());
		char symbol = line[line.size()-1];
		if (symbol == TokenCode::LFPAREN_CODE) brace_count++;
		if (symbol == TokenCode::RFPAREN_CODE) brace_count--;

		temp = lexer.getListOfTokens();
	}
	token_list_.insert(token_list_.end(), temp.begin(), temp.end()-1);
	lexer.clear();
}

// Initialize the lexer
void Lexer::initLexer(const std::string& code, const char* file) {
	stream_.initStream(code);
	file_name = file;
}

// Key in resreved words?
bool Lexer::isReservedKey(const std::string& key) {
	for (std::string el : reserved_words) { if (key == el) { return true; } }
	return false;
}

// Is symbol in resereved symbols?
bool Lexer::isReservedSymbol(char symbol) { 
	return reserved_symbols.find(stream_.getCurrentChar()) != reserved_symbols.end(); 
}

// Lex variable name or command
Token* Lexer::getId() {
	std::string temp;
	while (std::isalpha(stream_.getCurrentChar()) && stream_.hasNext()) { temp += stream_.getCurrentChar(); stream_.advance(); }
	if (isReservedKey(temp)) { return new Token(stream_.line, stream_.column - temp.length(), reserved_word_types.at(temp), temp); }
	return new Token(stream_.line, stream_.column - temp.length(), TokenType::ID, temp);
}

// Lex nums
Token* Lexer::getNum() {
	std::string temp;
	int dot_count = 0;
	while ((std::isdigit(stream_.getCurrentChar()) || stream_.currentCharEqual(TokenCode::DOT_CODE)) && stream_.hasNext()) {
		// lex float value
		if (stream_.currentCharEqual(TokenCode::DOT_CODE) && dot_count == 0) {
			while (std::isdigit(stream_.getCurrentChar() && stream_.hasNext())) {
				temp += stream_.getCurrentChar();
				stream_.advance();
			}
			dot_count++; 
		} else { // lex int value
			temp += stream_.getCurrentChar();
			stream_.advance();
		}
	}
	return new Token(stream_.line, stream_.column - temp.length(), ((dot_count > 0) ? TokenType::FLOAT : TokenType::INT), temp);
}

// Lex string
Token* Lexer::getString() { 
	std::string temp;
	while (stream_.hasNext()) {
		temp += stream_.getCurrentChar();
		stream_.advance();
		if (stream_.currentCharEqual(TokenCode::QUOTE_CODE)) { temp += stream_.getCurrentChar(); stream_.advance(); break; }
	}
	return new Token(stream_.line, stream_.column - temp.length(), TokenType::STRING, temp);
}

// Lex symbols
Token* Lexer::getSymbol() {
	Token* token;
	// <= >= == != += -= /= *= 
	if (stream_.currcharInList({ 
		TokenCode::LESS_CODE, TokenCode::GREATER_CODE, TokenCode::EQUAL_CODE, 
		TokenCode::NOT_CODE, TokenCode::PLUS_CODE, TokenCode::MINUS_CODE,
		TokenCode::SLASH_CODE, TokenCode::MULTIPLY_CODE
		}) && stream_.nextCharEqual(TokenCode::EQUAL_CODE)){
		std::string symbol = std::string(1, stream_.getCurrentChar()) + stream_.peekNextChar();
		token = new Token(stream_.line, stream_.column - 1, resreved_binary_operation_.at(symbol), symbol);
		stream_.advance(2);
	}

	// && ||
	else if ((stream_.currentCharEqual(TokenCode::LOGIC_AND_CODE) && stream_.nextCharEqual(TokenCode::LOGIC_AND_CODE) ||
		(stream_.currentCharEqual(TokenCode::LOGIC_OR_CODE) && stream_.nextCharEqual(TokenCode::LOGIC_OR_CODE)))) {
		std::string symbol = std::string(1, stream_.getCurrentChar()) + stream_.peekNextChar();
		token = new Token(stream_.line, stream_.column - 1, resreved_binary_operation_.at(symbol), symbol);
		stream_.advance(2);
	}
	else {
		if (stream_.currentCharEqual(TokenCode::LFPAREN_CODE) && stream_.nextCharEqual(TokenCode::EOF_CODE)) state_ = LexerState::IN_BLOCK;
		token = new Token(stream_.line, stream_.column - 1, reserved_symbols.at(stream_.getCurrentChar()), std::string(1, stream_.getCurrentChar()));
		stream_.advance();
	}
	return token;
}

// Main function
Token* Lexer::Lex() {
	while (stream_.hasNext() || state_ == LexerState::IN_BLOCK) {
		if (stream_.isSpace()) { stream_.skipSpace(); }
		if (stream_.currentCharEqual(TokenCode::SLASH_CODE) && stream_.nextCharEqual(TokenCode::SLASH_CODE)) { stream_.skipComments(); break; }
		if (std::isalpha(stream_.getCurrentChar())) { return getId(); }
		if (std::isdigit(stream_.getCurrentChar()) || stream_.currentCharEqual(TokenCode::DOT_CODE)) { return getNum(); }
		if (stream_.currentCharEqual(TokenCode::QUOTE_CODE)) { return getString(); }
		if (isReservedSymbol(stream_.getCurrentChar())) { return getSymbol(); }
		if (state_ == LexerState::IN_BLOCK) { getBlockOfCode(); }
		else { throw LexicalError(*this); return nullptr; }
		stream_.advance();
	}
	return new Token(stream_.line, stream_.column, TokenType::END_OF_FILE, "EOF");
}

std::vector<Token*> Lexer::getListOfTokens() {
	Token* token = Lex();
	while (token) {
		token_list_.push_back(token);
		if (token->type == TokenType::END_OF_FILE) { break; }
		token = Lex();
	}
	return token_list_;
}