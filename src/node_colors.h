#pragma once

#include "node_base.h"

namespace cse {

	class MixRGBNode : public EditableNode {
	public:
		MixRGBNode(FloatPos position);
	};

	class InvertNode : public EditableNode {
	public:
		InvertNode(FloatPos position);
	};

	class LightFalloffNode : public EditableNode {
	public:
		LightFalloffNode(FloatPos position);
	};

	class HSVNode : public EditableNode {
	public:
		HSVNode(FloatPos position);
	};

	class GammaNode : public EditableNode {
	public:
		GammaNode(FloatPos position);
	};

	class BrightnessContrastNode : public EditableNode {
	public:
		BrightnessContrastNode(FloatPos position);
	};

	class RGBCurvesNode : public EditableNode {
	public:
		RGBCurvesNode(FloatPos position);

		virtual void update_output_node(OutputNode& output) override;
	};

}
