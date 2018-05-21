#include "util_platform.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <nanovg.h>

CyclesShaderEditor::PathString CyclesShaderEditor::get_pathstring(const std::string& input)
{
#ifdef _WIN32
	return std::wstring(input.begin(), input.end());
#else
	return input;
#endif
}

CyclesShaderEditor::PathString CyclesShaderEditor::get_font_path(const PathString& search_path, const std::string& filename)
{
#ifdef _WIN32
	wchar_t PATH_SEPARATOR = L'\\';
#else
	char PATH_SEPARATOR = '/';
#endif
	return search_path + PATH_SEPARATOR + get_pathstring(filename);
}

void CyclesShaderEditor::nvg_create_font(const PathString& path, const std::string& name, NVGcontext* const nvg_context)
{
#ifdef _WIN32
	nvgCreateFontW(nvg_context, name.c_str(), path.c_str());
#else
	nvgCreateFont(nvg_context, name.c_str(), path.c_str());
#endif
}

void CyclesShaderEditor::thread_usleep(const int us)
{
#ifdef _WIN32
	Sleep(static_cast<DWORD>(us / 1000));
#else
	usleep(us);
#endif
}
