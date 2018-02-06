#pragma once

#include "node_base.h"

namespace CyclesShaderEditor {

	class MixRGBNode : public EditorNode {
	public:
		MixRGBNode(Point2 position);
	};

	class InvertNode : public EditorNode {
	public:
		InvertNode(Point2 position);
	};

	class LightFalloffNode : public EditorNode {
	public:
		LightFalloffNode(Point2 position);
	};

	class HSVNode : public EditorNode {
	public:
		HSVNode(Point2 position);
	};

	class GammaNode : public EditorNode {
	public:
		GammaNode(Point2 position);
	};

	class BrightnessContrastNode : public EditorNode {
	public:
		BrightnessContrastNode(Point2 position);
	};

	class RGBCurvesNode : public EditorNode {
	public:
		RGBCurvesNode(Point2 position);

		virtual void update_output_node(OutputNode& output) override;
	};

}
