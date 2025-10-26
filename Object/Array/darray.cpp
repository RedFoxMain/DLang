#include "darray.h"

std::string DArray::stringRepresentation() {
	if (m_data.empty()) return std::string();
	std::string temp;
	for (int i = 0; i < m_data.size(); ++i)
		temp += m_data[i]->value + ((i == m_data.size() - 1) ? '\0' : ',');
	return temp;
}

bool DArray::validateArray(std::vector<Token*> token_array) {
	TokenType token_type = token_array[0]->type;
	for (int i = 0; i < m_data.size(); ++i)
		if (m_data[i]->type != token_type) return false;
	return true;
}