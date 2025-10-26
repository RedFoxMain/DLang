#ifndef DARRAY_H
#define DARRAY_H

#include <iostream>
#include <vector>
#include <string>
#include "../../Parser/Lexer/lexer.h"
#include "../../Error/error.h"

class DArray { // I'LL RETURN HERE WHEN I START IMPLEMENTING THE INTERPRETER CLASS
private:
	std::vector<Token*> m_data;

public:
	DArray(std::vector<Token*> token_array): m_data(token_array) {
		if (!validateArray(m_data)) {
			raiseError(std::format("SYNTAX ERROR: All array elements must have one type! Error in line {}\n", m_data[0]->line));
			m_data.clear();
		}
	}
	std::string stringRepresentation();

private:
	bool validateArray(std::vector<Token*> token_array);
};
#endif // !DARRAY_H