#pragma once

#include "node_base.h"

namespace CyclesShaderEditor {

	class FloatPos;

	class BumpNode : public EditorNode {
	public:
		BumpNode(FloatPos position);
	};

	class NormalMapNode : public EditorNode {
	public:
		NormalMapNode(FloatPos position);
	};

	class VectorTransformNode : public EditorNode {
	public:
		VectorTransformNode(FloatPos position);
	};

}
