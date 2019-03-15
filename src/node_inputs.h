#pragma once

#include "node_base.h"

namespace cse {

	class FloatPos;

	class LightPathNode : public EditableNode {
	public:
		LightPathNode(FloatPos position);
	};

	class FresnelNode : public EditableNode {
	public:
		FresnelNode(FloatPos position);
	};

	class LayerWeightNode : public EditableNode {
	public:
		LayerWeightNode(FloatPos position);
	};

	class CameraDataNode : public EditableNode {
	public:
		CameraDataNode(FloatPos position);
	};

	class TangentNode : public EditableNode {
	public:
		TangentNode(FloatPos position);
	};

	class TextureCoordinateNode : public EditableNode {
	public:
		TextureCoordinateNode(FloatPos position);
	};

	class GeometryNode : public EditableNode {
	public:
		GeometryNode(FloatPos position);
	};

	class ObjectInfoNode : public EditableNode {
	public:
		ObjectInfoNode(FloatPos position);
	};

	class RGBNode : public EditableNode {
	public:
		RGBNode(FloatPos position);
	};

	class ValueNode : public EditableNode {
	public:
		ValueNode(FloatPos position);
	};

	class WireframeNode : public EditableNode {
	public:
		WireframeNode(FloatPos position);
	};
}
