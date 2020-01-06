#pragma once

#include "node_base.h"

namespace cse {

	class BrickTextureNode : public EditableNode {
	public:
		BrickTextureNode(Float2 position);
	};

	class NoiseTextureNode : public EditableNode {
	public:
		NoiseTextureNode(Float2 position);
	};

	class WaveTextureNode : public EditableNode {
	public:
		WaveTextureNode(Float2 position);
	};

	class VoronoiTextureNode : public EditableNode {
	public:
		VoronoiTextureNode(Float2 position);
	};

	class MusgraveTextureNode : public EditableNode {
	public:
		MusgraveTextureNode(Float2 position);
	};

	class GradientTextureNode : public EditableNode {
	public:
		GradientTextureNode(Float2 position);
	};

	class MagicTextureNode : public EditableNode {
	public:
		MagicTextureNode(Float2 position);
	};

	class CheckerTextureNode : public EditableNode {
	public:
		CheckerTextureNode(Float2 position);
	};

}

