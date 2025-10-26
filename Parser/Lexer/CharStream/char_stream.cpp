#include "char_stream.h"
#include "../../Tokens/tokens.h"

// Initialize the char stream
void CharStream::initStream(const std::string& str) {
	this->code = str;
	this->line = 1;
	this->column = -1;
	advance(); 
}

// Get next char
char CharStream::peekNextChar() { 
	return (column + 1 < code.length()) ? code[column + 1] : '\0'; 
}

char CharStream::peekPrevChar() {
	return (column - 1 > 0) ? code[column - 1] : '\0';
}

// Is current char equal ascii code of symbol
bool CharStream::currentCharEqual(int code) { 
	return current_char == code;
}

// check if current char in list of chars
bool CharStream::currcharInList(std::vector<int> list) {
	return std::find_if(list.begin(), list.end(), [=](char element) { return currentCharEqual(element); }) != list.end();
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

// Is current char is digit
bool CharStream::isDigit() {
	return std::isdigit(current_char);
}

// Is current char is alpha
bool CharStream::isAlpha() {
	return std::isalpha(current_char);
}

bool CharStream::isEndOfString() {
	return current_char == '\0';
}

// Next char
void CharStream::advance(int step) {
	column += step;
	current_char = (column < code.length()) ? code[column] : '\0';
	if (current_char == '\n') ++line;
}

// Skip the comments
void CharStream::skipComments() { 
	while (hasNext() && current_char != '\n') advance();
}

// Skip space
void CharStream::skipSpace() { 
	while (isSpace()) advance();
}