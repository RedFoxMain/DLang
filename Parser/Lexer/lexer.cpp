#include "Lexer.h"
#include "../../Error/error.h"

// Key in resreved words?
bool Lexer::isReservedKey(const std::string& key) {
	return std::find(reserved_words.begin(), reserved_words.end(), key) != reserved_words.end();
}

// Is symbol in resereved symbols?
bool Lexer::isReservedSymbol(char symbol) { 
	return reserved_symbols.find(stream.current_char) != reserved_symbols.end(); 
}

// Lex variable name or command
Token* Lexer::getId() {
	std::string temp;
	while (stream.isAlpha() && stream.hasNext()) { temp += stream.current_char; stream.advance(); }
	if (isReservedKey(temp)) { return new Token(stream.line, stream.column - temp.length(), reserved_word_types.at(temp), temp); }
	return new Token(stream.line, stream.column - temp.length(), TokenType::ID, temp);
}

// Lex nums
Token* Lexer::getNum() {
	std::string temp;
	int dot_count = 0;
	while ((stream.isDigit() || stream.currentCharEqual(TokenCode::DOT_CODE)) && stream.hasNext()) {
		// lex float value
		if (stream.currentCharEqual(TokenCode::DOT_CODE) && dot_count == 0) {
			while (std::isdigit(stream.current_char && stream.hasNext())) {
				temp += stream.current_char;
				stream.advance();
			}
			dot_count++; 
		} else { // lex int value
			temp += stream.current_char;
			stream.advance();
		}
	}
	return new Token(stream.line, stream.column - temp.length(), ((dot_count > 0) ? TokenType::FLOAT : TokenType::INT), temp);
}

// Lex string
Token* Lexer::getString() { 
	std::string temp;
	while (stream.hasNext()) {
		temp += stream.current_char;
		stream.advance();
		if (stream.currentCharEqual(TokenCode::QUOTE_CODE)) { temp += stream.current_char; stream.advance(); break; }
	}
	return new Token(stream.line, stream.column - temp.length(), TokenType::STRING, temp);
}

// Lex symbols
Token* Lexer::getSymbol() {
	Token* token;
	// <= >= == != += -= /= *= 
	if (stream.currcharInList({ 
		TokenCode::LESS_CODE, TokenCode::GREATER_CODE, TokenCode::EQUAL_CODE, 
		TokenCode::NOT_CODE, TokenCode::PLUS_CODE, TokenCode::MINUS_CODE,
		TokenCode::SLASH_CODE, TokenCode::MULTIPLY_CODE
		}) && stream.nextCharEqual(TokenCode::EQUAL_CODE)){
		std::string symbol = std::string(1, stream.current_char) + stream.peekNextChar();
		token = new Token(stream.line, stream.column - 1, resreved_binary_operation_.at(symbol), symbol);
		stream.advance(2);
	}
	// && ||
	else if ((stream.currentCharEqual(TokenCode::LOGIC_AND_CODE) && stream.nextCharEqual(TokenCode::LOGIC_AND_CODE) ||
		(stream.currentCharEqual(TokenCode::LOGIC_OR_CODE) && stream.nextCharEqual(TokenCode::LOGIC_OR_CODE)))) {
		std::string symbol = std::string(1, stream.current_char) + stream.peekNextChar();
		token = new Token(stream.line, stream.column - 1, resreved_binary_operation_.at(symbol), symbol);
		stream.advance(2);
	}
	else {
		token = new Token(stream.line, stream.column - 1, reserved_symbols.at(stream.current_char), std::string(1, stream.current_char));
		stream.advance();
	}
	return token;
}

Token* Lexer::getToken() {
	while (stream.hasNext()) {
		while (stream.hasNext()) {
			if (stream.isSpace()) { stream.skipSpace(); }
			if (stream.currentCharEqual(TokenCode::SLASH_CODE) && stream.nextCharEqual(TokenCode::SLASH_CODE)) { stream.skipComments(); break; }
			if (std::isalpha(stream.current_char)) { return getId(); }
			if (std::isdigit(stream.current_char) || stream.currentCharEqual(TokenCode::DOT_CODE)) { return getNum(); }
			if (stream.currentCharEqual(TokenCode::QUOTE_CODE)) { return getString(); }
			if (isReservedSymbol(stream.current_char)) { return getSymbol(); }
			else { stream.advance(); continue; }
			stream.advance();
		}
	}
	return new Token(stream.line, stream.column, TokenType::END_OF_FILE, "EOF");
}

// Main function
std::vector<Token*> Lexer::lex(const std::vector<std::string> code, const char* file) {
	std::string line;
	for (std::string str: code) {
		line += str + "\n";
	}
	//std::cout << line;
	this->stream.initStream(line);
	this->file_name = file;
	m_tokens.clear();
	Token* token = getToken();
	while (token) {
		m_tokens.push_back(token);
		if (token->type == TokenType::END_OF_FILE) 
			break;
		token = getToken();
	}
	return m_tokens;
} 