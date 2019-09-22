#ifndef TYPE_TO_STRING_H
#define TYPE_TO_STRING_H

#include <string>

class TypeToString
{
public:
	virtual std::string Convert(int enumeration) = 0;
};

#endif