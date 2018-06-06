#pragma once

#include "node_base.h"

namespace CyclesShaderEditor {

	class FloatPos;

	class LightPathNode : public EditorNode {
	public:
		LightPathNode(FloatPos position);
	};

	class FresnelNode : public EditorNode {
	public:
		FresnelNode(FloatPos position);
	};

	class LayerWeightNode : public EditorNode {
	public:
		LayerWeightNode(FloatPos position);
	};

	class CameraDataNode : public EditorNode {
	public:
		CameraDataNode(FloatPos position);
	};

	class TangentNode : public EditorNode {
	public:
		TangentNode(FloatPos position);
	};

	class TextureCoordinateNode : public EditorNode {
	public:
		TextureCoordinateNode(FloatPos position);
	};

	class GeometryNode : public EditorNode {
	public:
		GeometryNode(FloatPos position);
	};

	class ObjectInfoNode : public EditorNode {
	public:
		ObjectInfoNode(FloatPos position);
	};

	class RGBNode : public EditorNode {
	public:
		RGBNode(FloatPos position);
	};

	class ValueNode : public EditorNode {
	public:
		ValueNode(FloatPos position);
	};

	class WireframeNode : public EditorNode {
	public:
		WireframeNode(FloatPos position);
	};
}