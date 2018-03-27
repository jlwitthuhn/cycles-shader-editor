#include "util_platform.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

CyclesShaderEditor::PathString CyclesShaderEditor::get_pathstring(const std::string& input)
{
#ifdef _WIN32
	return std::wstring(input.begin(), input.end());
#else
	return input;
#endif
}

void CyclesShaderEditor::thread_usleep(int us)
{
#ifdef _WIN32
	Sleep(static_cast<DWORD>(us / 1000));
#else
	usleep(us);
#endif
}
