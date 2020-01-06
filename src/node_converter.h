#pragma once

#include "node_base.h"

namespace cse {

	class BlackbodyNode : public EditableNode {
	public:
		BlackbodyNode(Float2 position);
	};

	class ColorRampNode : public EditableNode {
	public:
		ColorRampNode(Float2 position);

		virtual void update_output_node(OutputNode& output) override;
	};

	class CombineHSVNode : public EditableNode {
	public:
		CombineHSVNode(Float2 position);
	};

	class CombineRGBNode : public EditableNode {
	public:
		CombineRGBNode(Float2 position);
	};

	class CombineXYZNode : public EditableNode {
	public:
		CombineXYZNode(Float2 position);
	};

	class MathNode : public EditableNode {
	public:
		MathNode(Float2 position);
	};

	class RGBToBWNode : public EditableNode {
	public:
		RGBToBWNode(Float2 position);
	};

	class SeparateHSVNode : public EditableNode {
	public:
		SeparateHSVNode(Float2 position);
	};

	class SeparateRGBNode : public EditableNode {
	public:
		SeparateRGBNode(Float2 position);
	};

	class SeparateXYZNode : public EditableNode {
	public:
		SeparateXYZNode(Float2 position);
	};

	class VectorMathNode : public EditableNode {
	public:
		VectorMathNode(Float2 position);
	};

	class WavelengthNode : public EditableNode {
	public:
		WavelengthNode(Float2 position);
	};

}
