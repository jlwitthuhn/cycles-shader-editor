#include "wrapper_nvg_context.h"

#define NANOVG_GL2_IMPLEMENTATION

#include <GL/glew.h>
#include <nanovg_gl.h>

cse::NvgContext::NvgContext() :
	context_ptr(nvgCreateGL2(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG))
{

}

cse::NvgContext::~NvgContext()
{
	if (context_ptr != nullptr) {
		nvgDeleteGL2(context_ptr);
	}
}

bool cse::NvgContext::is_valid() const {
	return (context_ptr != nullptr);
}
