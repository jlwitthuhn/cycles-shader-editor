#pragma once

// This header defines small inline-able functions to make standard nanovg functions work with
// this project's internal types

#include <memory>

#include <nanovg.h>

#include "util_vector.h"
#include "wrapper_nvg_context.h"

namespace cse {

	// Frame begin/end control
	inline void nvgBeginFrame(const std::unique_ptr<NvgContext>& ctx, const float windowWidth, const float windowHeight, const float devicePixelRatio) {
		nvgBeginFrame(ctx->context_ptr, windowWidth, windowHeight, devicePixelRatio);
	}
	inline void nvgEndFrame(const std::unique_ptr<NvgContext>& ctx) {
		nvgEndFrame(ctx->context_ptr);
	}

	// Color stuff
	inline void nvgFillColor(NVGcontext* const ctx, const Float3& color) {
		nvgFillColor(ctx, nvgRGBf(color.x, color.y, color.z));
	}
}
