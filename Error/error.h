#ifndef ERROR_H
#define ERROR_H

#include <iostream>
#include <exception>
#include <format>

static void raiseError(const std::string& msg) {
	throw std::runtime_error(msg);
}
#endif // !ERROR_H