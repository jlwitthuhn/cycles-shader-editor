#pragma once

#include "node_base.h"

namespace cse {

	class Float2;

	struct OutputNode;

	class MixRGBNode : public EditableNode {
	public:
		MixRGBNode(Float2 position);
	};

	class InvertNode : public EditableNode {
	public:
		InvertNode(Float2 position);
	};

	class LightFalloffNode : public EditableNode {
	public:
		LightFalloffNode(Float2 position);
	};

	class HSVNode : public EditableNode {
	public:
		HSVNode(Float2 position);
	};

	class GammaNode : public EditableNode {
	public:
		GammaNode(Float2 position);
	};

	class BrightnessContrastNode : public EditableNode {
	public:
		BrightnessContrastNode(Float2 position);
	};

	class RGBCurvesNode : public EditableNode {
	public:
		RGBCurvesNode(Float2 position);

		virtual void update_output_node(OutputNode& output) override;
	};

}
