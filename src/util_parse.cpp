#include "util_parse.h"

#include <sstream>

float cse::locale_safe_stof(std::string input)
{
	std::stringstream stream(input);
	float result = 0.0f;
	stream >> result;
	return result;
}
