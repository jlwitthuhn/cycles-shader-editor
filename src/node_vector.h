#pragma once

#include "node_base.h"

namespace cse {

	class Float2;

	class BumpNode : public EditableNode {
	public:
		BumpNode(Float2 position);
	};

	class DisplacementNode : public EditableNode {
	public:
		DisplacementNode(Float2 position);
	};

	class NormalMapNode : public EditableNode {
	public:
		NormalMapNode(Float2 position);
	};

	class VectorTransformNode : public EditableNode {
	public:
		VectorTransformNode(Float2 position);
	};

}
