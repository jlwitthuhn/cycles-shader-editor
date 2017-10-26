#pragma once

#include "node_base.h"

namespace CyclesShaderEditor {

	class LightPathNode : public EditorNode {
	public:
		LightPathNode(Point2 position);
	};

	class FresnelNode : public EditorNode {
	public:
		FresnelNode(Point2 position);
	};

	class LayerWeightNode : public EditorNode {
	public:
		LayerWeightNode(Point2 position);
	};

	class CameraDataNode : public EditorNode {
	public:
		CameraDataNode(Point2 position);
	};

	class TangentNode : public EditorNode {
	public:
		TangentNode(Point2 position);
	};

	class TextureCoordinateNode : public EditorNode {
	public:
		TextureCoordinateNode(Point2 position);
	};

}