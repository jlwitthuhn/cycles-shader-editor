#pragma once

#include "node_base.h"

namespace cse {

	class FloatPos;

	class BlackbodyNode : public EditableNode {
	public:
		BlackbodyNode(FloatPos position);
	};

	class ColorRampNode : public EditableNode {
	public:
		ColorRampNode(FloatPos position);

		virtual void update_output_node(OutputNode& output) override;
	};

	class CombineHSVNode : public EditableNode {
	public:
		CombineHSVNode(FloatPos position);
	};

	class CombineRGBNode : public EditableNode {
	public:
		CombineRGBNode(FloatPos position);
	};

	class CombineXYZNode : public EditableNode {
	public:
		CombineXYZNode(FloatPos position);
	};

	class MathNode : public EditableNode {
	public:
		MathNode(FloatPos position);
	};

	class RGBToBWNode : public EditableNode {
	public:
		RGBToBWNode(FloatPos position);
	};

	class SeparateHSVNode : public EditableNode {
	public:
		SeparateHSVNode(FloatPos position);
	};

	class SeparateRGBNode : public EditableNode {
	public:
		SeparateRGBNode(FloatPos position);
	};

	class SeparateXYZNode : public EditableNode {
	public:
		SeparateXYZNode(FloatPos position);
	};

	class VectorMathNode : public EditableNode {
	public:
		VectorMathNode(FloatPos position);
	};

	class WavelengthNode : public EditableNode {
	public:
		WavelengthNode(FloatPos position);
	};

}
