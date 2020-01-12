#pragma once

// This header defines small inline-able functions to make standard nanovg functions work with
// this project's internal types

#include <memory>

#include <nanovg.h>

#include "util_vector.h"
#include "wrapper_nvg_context.h"

namespace cse {

	// Frame begin/end control
	inline void nvgBeginFrame(const std::unique_ptr<NvgContext>& ctx, const float windowWidth, const float windowHeight, const float devicePixelRatio)
	{
		nvgBeginFrame(ctx->context_ptr, windowWidth, windowHeight, devicePixelRatio);
	}

	inline void nvgEndFrame(const std::unique_ptr<NvgContext>& ctx)
	{
		nvgEndFrame(ctx->context_ptr);
	}

	// Path
	inline void nvgRect(NVGcontext* const ctx, const Float2 begin, const Float2 size)
	{
		nvgRect(ctx, begin.x, begin.y, size.x, size.y);
	}

	// Color
	inline void nvgFillColor(NVGcontext* const ctx, const Float3 color)
	{
		nvgFillColor(ctx, nvgRGBf(color.x, color.y, color.z));
	}

	inline NVGpaint nvgLinearGradient(
		NVGcontext* const ctx,
		const float sx,
		const float sy,
		const float ex,
		const float ey,
		const Float3 icol,
		const Float3 ocol)
	{
		return nvgLinearGradient(ctx, sx, sy, ex, ey, nvgRGBf(icol.x, icol.y, icol.z), nvgRGBf(ocol.x, ocol.y, ocol.z));
	}
}
