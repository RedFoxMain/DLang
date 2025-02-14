#ifndef STRING_OBJECT_H
#define STRING_OBJECT_H

#include <string>
class String {
private:
	std::string obj_;

public:
	String(const std::string& obj): obj_(obj) {}
	std::string getSelf() { return obj_; }
};
#endif
