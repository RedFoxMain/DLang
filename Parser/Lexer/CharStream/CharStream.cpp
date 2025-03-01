#include "CharStream.h"
#include "../../Tokens/tokens.h"

// Return current char
char CharStream::getCurrentChar() { return current_char; }

// Initialize the char stream
void CharStream::initStream(const std::string& str, const char* file) {
	this->code = str;
	this->file = file;
	resetLine();
	advance(); 
}

// Get next char
char CharStream::peekNextChar() { 
	return (column+1 < code.length()) ? code[column+1] : '\0'; 
}

// Is current char equal ascii code of symbol
bool CharStream::currentCharEqual(int code) { 
	return current_char == code;
}

// Is next char equal ascii code of symbol
bool CharStream::nextCharEqual(int code) { 
	return peekNextChar() == code; 
}

// Check if current char equal \0
bool CharStream::hasNext() { 
	return current_char != '\0';
}

// Is current char is space
bool CharStream::isSpace() { 
	return std::isspace(current_char);
}

// Next char
void CharStream::advance(int step) {
	column += step;
	current_char = (column < code.length()) ? code[column] : '\0';
}

// Skip the comments
void CharStream::skipComments() { 
	while (hasNext()) { advance(); } 
}

// Skip space
void CharStream::skipSpace() { 
	while (isSpace()) { advance(); } 
}

// Increase line and reset column
void CharStream::resetLine() {
	line++; column = -1;
}