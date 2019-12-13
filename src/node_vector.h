#pragma once

#include "node_base.h"

namespace cse {

	class BumpNode : public EditableNode {
	public:
		BumpNode(FloatPos position);
	};

	class DisplacementNode : public EditableNode {
	public:
		DisplacementNode(FloatPos position);
	};

	class NormalMapNode : public EditableNode {
	public:
		NormalMapNode(FloatPos position);
	};

	class VectorTransformNode : public EditableNode {
	public:
		VectorTransformNode(FloatPos position);
	};

}
