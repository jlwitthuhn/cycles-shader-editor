#pragma once

#include "node_base.h"

namespace CyclesShaderEditor {

	class FloatPos;

	struct OutputNode;

	class MixRGBNode : public EditorNode {
	public:
		MixRGBNode(FloatPos position);
	};

	class InvertNode : public EditorNode {
	public:
		InvertNode(FloatPos position);
	};

	class LightFalloffNode : public EditorNode {
	public:
		LightFalloffNode(FloatPos position);
	};

	class HSVNode : public EditorNode {
	public:
		HSVNode(FloatPos position);
	};

	class GammaNode : public EditorNode {
	public:
		GammaNode(FloatPos position);
	};

	class BrightnessContrastNode : public EditorNode {
	public:
		BrightnessContrastNode(FloatPos position);
	};

	class RGBCurvesNode : public EditorNode {
	public:
		RGBCurvesNode(FloatPos position);

		virtual void update_output_node(OutputNode& output) override;
	};

}
