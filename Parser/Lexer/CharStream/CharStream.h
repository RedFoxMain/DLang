#ifndef CHAR_STREAM_H
#define CHAR_STREAM_H

#include <string>
#include <cctype>

class CharStream {
public:
	size_t line = 0, column = -1;
	std::string code;
	std::string file;
	char current_char;	

public:
	CharStream() = default;
	void initStream(const std::string& str, const char* file="<stdin>"); // Init stream
	void advance(int step = 1); // Move to next char
	void skipComments(); // Skip the comments
	void skipSpace(); // Skip space
	void resetLine(); // Increase line and reset column
	char getCurrentChar(); // Get current char
	char peekNextChar(); // Get next char
	bool currentCharEqual(int code); // Is current char equal ascii code of symbol
	bool nextCharEqual(int code); // Is next char equal ascii code of symbol
	bool hasNext(); // Check if line is not ended
	bool isSpace(); // Is current char is space
};
#endif // !CHAR_STREAM_H