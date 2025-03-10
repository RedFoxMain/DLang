#ifndef TOKENS_H
#define TOKENS_H

enum TokenType { 
	END_OF_FILE, NONE, INT, FLOAT, 
	STRING, BOOL, CHAR, VAR_KEYWORD, CONST_KEYWORD,
	WHILE_KEYWORD, FOR_KEYWORD, FOREACH_KEYWORD, 
	IF_KEYWORD, ELSE_KEYWORD, FUNC_KEYWORD, VARIABLE_TYPE, BLOCK,
	ID, SEMICOLON, COLON, EQUAL, 
	DOT, QUOTE, SLASH, LESS, 
	GREATER, NOT, GREATER_EQUAL, LESS_EQUAL, 
	EQUAL_EQUAL, NOT_EQUAL, PLUS, MINUS, DIVIDE, 
	MULTIPLY, PLUS_EQUAL, MINUS_EQUAL, DIVIDE_EQUAL, MULTIPLY_EQUAL,
	LRPAREN, RRPAREN, LFPAREN, RFPAREN, LOGIC_AND, LOGIC_OR, 
	UNARY_LOGIC_OR, UNARY_LOGIC_AND
};

enum TokenCode {
	EOF_CODE = 0,
	SEMICOLON_CODE = 59,
	COLON_CODE = 58,
	QUOTE_CODE = 34,
	EQUAL_CODE = 61,
	DOT_CODE = 46,
	SLASH_CODE = 47,
	LESS_CODE = 60,
	GREATER_CODE = 62,
	NOT_CODE = 33,
	PLUS_CODE = 43,
	MINUS_CODE = 45,
	MULTIPLY_CODE = 42,
	LRPAREN_CODE = 40,
	RRPAREN_CODE = 41,
	LFPAREN_CODE = 123,
	RFPAREN_CODE = 125,
	LOGIC_OR_CODE = 124,
	LOGIC_AND_CODE = 38
};
#endif // !TOKENS_H