#pragma once

#include "node_base.h"

namespace CyclesShaderEditor {

	class FloatPos;

	class BrickTextureNode : public EditorNode {
	public:
		BrickTextureNode(FloatPos position);
	};

	class NoiseTextureNode : public EditorNode {
	public:
		NoiseTextureNode(FloatPos position);
	};

	class WaveTextureNode : public EditorNode {
	public:
		WaveTextureNode(FloatPos position);
	};

	class VoronoiTextureNode : public EditorNode {
	public:
		VoronoiTextureNode(FloatPos position);
	};

	class MusgraveTextureNode : public EditorNode {
	public:
		MusgraveTextureNode(FloatPos position);
	};

	class GradientTextureNode : public EditorNode {
	public:
		GradientTextureNode(FloatPos position);
	};

	class MagicTextureNode : public EditorNode {
	public:
		MagicTextureNode(FloatPos position);
	};

	class CheckerTextureNode : public EditorNode {
	public:
		CheckerTextureNode(FloatPos position);
	};

}

