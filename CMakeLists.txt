cmake_minimum_required (VERSION 3.8)
set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)

project("DLang")

set( INCLUDE_FILES
	"Interpreter/interpreter.h"
	"Object/Array/darray.h"
	"Parser/AST/ast.h"
	"Parser/AST/ast_printer.h"
	"Parser/Lexer/CharStream/char_stream.h"
	"Parser/Lexer/lexer.h"
	"Parser/Tokens/tokens.h"
	"Parser/parser.h"
	
)

set( SRC_FILES
	"Interpreter/shell.cpp"
	"Object/Array/darray.cpp"
	"Parser/Lexer/CharStream/char_stream.cpp"
	"Parser/Lexer/lexer.cpp"
	"Parser/parser.cpp"
)
add_executable(${PROJECT_NAME} ${SRC_FILES} ${INCLUDE_FILES})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)