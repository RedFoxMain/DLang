#ifndef CHAR_STREAM_H
#define CHAR_STREAM_H

#include <string>
#include <cctype>
#include <vector>

class CharStream {
public:
	int line;
	int column;
	std::string code;
	char current_char;	

public:
	CharStream(): line(0), column(-1) {}
	void initStream(const std::string& str); // Init stream
	void advance(int step = 1); // Move to next char
	void skipComments(); // Skip the comments
	void skipSpace(); // Skip space
	char peekNextChar(); // Get next char
	char peekPrevChar(); // Get prev char

	bool currentCharEqual(int code); // Is current char equal ascii code of symbol
	bool nextCharEqual(int code); // Is next char equal ascii code of symbol
	bool hasNext(); // Check if line is not ended
	bool isSpace(); // Is current char is space
	bool isDigit(); // Is current char is digit
	bool isAlpha(); // Is current char is alpha
	bool currcharInList(std::vector<int> list); // check if current char in list of char
};
#endif // !CHAR_STREAM_H