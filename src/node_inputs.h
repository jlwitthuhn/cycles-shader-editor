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

	class GeometryNode : public EditorNode {
	public:
		GeometryNode(Point2 position);
	};

	class ObjectInfoNode : public EditorNode {
	public:
		ObjectInfoNode(Point2 position);
	};

	class RGBNode : public EditorNode {
	public:
		RGBNode(Point2 position);
	};

	class ValueNode : public EditorNode {
	public:
		ValueNode(Point2 position);
	};

	class WireframeNode : public EditorNode {
	public:
		WireframeNode(Point2 position);
	};
}