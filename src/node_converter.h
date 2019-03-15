#pragma once

#include "node_base.h"

namespace cse {

	class FloatPos;

	class BlackbodyNode : public EditorNode {
	public:
		BlackbodyNode(FloatPos position);
	};

	class CombineHSVNode : public EditorNode {
	public:
		CombineHSVNode(FloatPos position);
	};

	class CombineRGBNode : public EditorNode {
	public:
		CombineRGBNode(FloatPos position);
	};

	class CombineXYZNode : public EditorNode {
	public:
		CombineXYZNode(FloatPos position);
	};

	class MathNode : public EditorNode {
	public:
		MathNode(FloatPos position);
	};

	class RGBToBWNode : public EditorNode {
	public:
		RGBToBWNode(FloatPos position);
	};

	class SeparateHSVNode : public EditorNode {
	public:
		SeparateHSVNode(FloatPos position);
	};

	class SeparateRGBNode : public EditorNode {
	public:
		SeparateRGBNode(FloatPos position);
	};

	class SeparateXYZNode : public EditorNode {
	public:
		SeparateXYZNode(FloatPos position);
	};

	class VectorMathNode : public EditorNode {
	public:
		VectorMathNode(FloatPos position);
	};

	class WavelengthNode : public EditorNode {
	public:
		WavelengthNode(FloatPos position);
	};

}
