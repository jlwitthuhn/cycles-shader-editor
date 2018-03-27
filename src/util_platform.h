// Platform-specific things go in this header
#pragma once

#include <string>

namespace CyclesShaderEditor {
#ifdef _WIN32
	typedef std::wstring PathString;
#else
	typedef std::string PathString;
#endif
	PathString get_pathstring(const std::string& input);

	void thread_usleep(int us);
}
