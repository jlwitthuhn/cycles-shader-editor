// Platform-specific things go in this header
#pragma once

#include <string>

struct NVGcontext;

namespace CyclesShaderEditor {
#ifdef _WIN32
	typedef std::wstring PathString;
#else
	typedef std::string PathString;
#endif
	namespace Platform {
		PathString get_pathstring(const std::string& input);
		PathString get_font_path(const PathString& search_path, const std::string& filename);

		void nvg_create_font(const PathString& path, const std::string& name, NVGcontext* nvg_context);

		int get_delete_key();

		void thread_usleep(int us);
	}
}
