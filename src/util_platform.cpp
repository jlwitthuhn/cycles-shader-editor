#include "util_platform.h"

#ifdef _WIN32
#include <Windows.h>
void CyclesShaderEditor::thread_usleep(int us)
{
	Sleep(static_cast<DWORD>(us / 1000));
}
#elif __linux__
#include <unistd.h>
void CyclesShaderEditor::thread_usleep(int us)
{
	usleep(us);
}
#endif