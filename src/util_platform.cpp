#include "util_platform.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <GLFW/glfw3.h>
#include <nanovg.h>

#include "wrapper_nvg_context.h"

CyclesShaderEditor::PathString CyclesShaderEditor::Platform::get_pathstring(const std::string& input)
{
#ifdef _WIN32
	return std::wstring(input.begin(), input.end());
#else
	return input;
#endif
}

CyclesShaderEditor::PathString CyclesShaderEditor::Platform::get_font_path(const PathString& search_path, const std::string& filename)
{
#ifdef _WIN32
	wchar_t PATH_SEPARATOR = L'\\';
#else
	char PATH_SEPARATOR = '/';
#endif
	return search_path + PATH_SEPARATOR + get_pathstring(filename);
}

void CyclesShaderEditor::Platform::nvg_create_font(const PathString& path, const std::string& name, const std::unique_ptr<NvgContext>& nvg_context)
{
#ifdef _WIN32
	nvgCreateFontW(nvg_context->context_ptr, name.c_str(), path.c_str());
#else
	nvgCreateFont(nvg_context->context_ptr, name.c_str(), path.c_str());
#endif
}

int CyclesShaderEditor::Platform::get_delete_key()
{
#ifdef __APPLE__
	return GLFW_KEY_BACKSPACE;
#else
	return GLFW_KEY_DELETE;
#endif
}

void CyclesShaderEditor::Platform::thread_usleep(const int us)
{
#ifdef _WIN32
	Sleep(static_cast<DWORD>(us / 1000));
#else
	usleep(us);
#endif
}
