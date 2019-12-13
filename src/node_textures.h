#pragma once

#include "node_base.h"

namespace cse {

	class BrickTextureNode : public EditableNode {
	public:
		BrickTextureNode(FloatPos position);
	};

	class NoiseTextureNode : public EditableNode {
	public:
		NoiseTextureNode(FloatPos position);
	};

	class WaveTextureNode : public EditableNode {
	public:
		WaveTextureNode(FloatPos position);
	};

	class VoronoiTextureNode : public EditableNode {
	public:
		VoronoiTextureNode(FloatPos position);
	};

	class MusgraveTextureNode : public EditableNode {
	public:
		MusgraveTextureNode(FloatPos position);
	};

	class GradientTextureNode : public EditableNode {
	public:
		GradientTextureNode(FloatPos position);
	};

	class MagicTextureNode : public EditableNode {
	public:
		MagicTextureNode(FloatPos position);
	};

	class CheckerTextureNode : public EditableNode {
	public:
		CheckerTextureNode(FloatPos position);
	};

}

