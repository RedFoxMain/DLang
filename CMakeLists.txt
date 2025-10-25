cmake_minimum_required (VERSION 3.8)
set(CXX_STANDARD 17)

project("DLang")
set(PROJECT_NAME "DLang")

# The folders where the files are located
set( FLDS_NAMES
	"Parser/Lexer/CharStream" "Parser/Tokens" "Parser/Lexer" "Parser/AST"
	"Parser" "Error" "Interpreter"
)

# List for file to compile
set( FILE_TO_COMPILE )

foreach(fld_name ${FLDS_NAMES})
	file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/${fld_name}/*.cpp)
	file(GLOB_RECURSE HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/${fld_name}/*.h)
	list(APPEND FILE_TO_COMPILE ${CPP_FILES} )
	list(APPEND FILE_TO_COMPILE ${HEADER_FILES})
endforeach()

add_executable(${PROJECT_NAME} ${FILE_TO_COMPILE})