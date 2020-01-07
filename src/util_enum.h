#pragma once

namespace cse {

	enum class NodeCategory {
		OUTPUT,
		COLOR,
		CONVERTER,
		INPUT,
		SHADER,
		TEXTURE,
		VECTOR,
	};

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
		INPUT,
		OUTPUT,
	};

	enum class SocketType {
		FLOAT,
		INT,
		COLOR,
		VECTOR,
		NORMAL,
		CLOSURE,
		STRING,
		STRING_ENUM,
		BOOLEAN,
		CURVE,
		COLOR_RAMP,
	};
}
