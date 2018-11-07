#pragma once

#include <string>

namespace CyclesShaderEditor {

	// This will parse a string using the default locale rather than the application locale
	// It should be used for loading files
	float locale_safe_stof(std::string input);
}
