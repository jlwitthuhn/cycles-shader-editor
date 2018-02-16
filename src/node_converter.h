#pragma once

#include "node_base.h"

namespace CyclesShaderEditor {

	class BlackbodyNode : public EditorNode {
	public:
		BlackbodyNode(Point2 position);
	};

	class CombineHSVNode : public EditorNode {
	public:
		CombineHSVNode(Point2 position);
	};

	class CombineRGBNode : public EditorNode {
	public:
		CombineRGBNode(Point2 position);
	};

	class CombineXYZNode : public EditorNode {
	public:
		CombineXYZNode(Point2 position);
	};

	class MathNode : public EditorNode {
	public:
		MathNode(Point2 position);
	};

	class RGBToBWNode : public EditorNode {
	public:
		RGBToBWNode(Point2 position);
	};

	class SeparateHSVNode : public EditorNode {
	public:
		SeparateHSVNode(Point2 position);
	};

	class SeparateRGBNode : public EditorNode {
	public:
		SeparateRGBNode(Point2 position);
	};

	class SeparateXYZNode : public EditorNode {
	public:
		SeparateXYZNode(Point2 position);
	};

	class VectorMathNode : public EditorNode {
	public:
		VectorMathNode(Point2 position);
	};

	class WavelengthNode : public EditorNode {
	public:
		WavelengthNode(Point2 position);
	};

}
