#pragma once

#include "node_base.h"

namespace cse {

	class Float2;

	class AmbientOcculsionNode : public EditableNode {
	public:
		AmbientOcculsionNode(Float2 position);
	};

	class BevelNode : public EditableNode {
	public:
		BevelNode(Float2 position);
	};

	class CameraDataNode : public EditableNode {
	public:
		CameraDataNode(Float2 position);
	};

	class FresnelNode : public EditableNode {
	public:
		FresnelNode(Float2 position);
	};

	class GeometryNode : public EditableNode {
	public:
		GeometryNode(Float2 position);
	};

	class LayerWeightNode : public EditableNode {
	public:
		LayerWeightNode(Float2 position);
	};

	class LightPathNode : public EditableNode {
	public:
		LightPathNode(Float2 position);
	};

	class ObjectInfoNode : public EditableNode {
	public:
		ObjectInfoNode(Float2 position);
	};

	class RGBNode : public EditableNode {
	public:
		RGBNode(Float2 position);
	};

	class TangentNode : public EditableNode {
	public:
		TangentNode(Float2 position);
	};

	class TextureCoordinateNode : public EditableNode {
	public:
		TextureCoordinateNode(Float2 position);
	};

	class ValueNode : public EditableNode {
	public:
		ValueNode(Float2 position);
	};

	class WireframeNode : public EditableNode {
	public:
		WireframeNode(Float2 position);
	};
}
