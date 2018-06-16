#pragma once

namespace CyclesShaderEditor {

	enum class ShaderGraphType {
		EMPTY,
		MATERIAL,
	};

	enum class EditCurveMode {
		MOVE,
		CREATE,
		DELETE,
	};

	enum class CurveInterpolation {
		LINEAR,
		CUBIC_HERMITE,
	};

	enum class SocketIOType {
		Input,
		Output,
	};
}
