#pragma once

struct NVGcontext;

namespace cse {

	// Class to safely wrap nvgCreate and nvgDelete
	class NvgContext {
	public:
		NvgContext();
		~NvgContext();

		bool is_valid() const;

		NVGcontext* const context_ptr;
	};
}
