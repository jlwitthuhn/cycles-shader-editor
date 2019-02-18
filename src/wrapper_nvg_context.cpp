#include "wrapper_nvg_context.h"

#define NANOVG_GL2_IMPLEMENTATION

#include <GL/glew.h>
#include <nanovg.h>
#include <nanovg_gl.h>

CyclesShaderEditor::NvgContext::NvgContext() :
	context_ptr(nvgCreateGL2(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG))
{

}

CyclesShaderEditor::NvgContext::~NvgContext()
{
	if (context_ptr != nullptr) {
		nvgDeleteGL2(context_ptr);
	}
}

bool CyclesShaderEditor::NvgContext::is_valid() const {
	return (context_ptr != nullptr);
}
