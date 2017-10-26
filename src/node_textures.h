#pragma once

#include "node_base.h"

namespace CyclesShaderEditor {

	class NoiseTextureNode : public EditorNode {
	public:
		NoiseTextureNode(Point2 position);
	};

	class WaveTextureNode : public EditorNode {
	public:
		WaveTextureNode(Point2 position);
	};

	class VoronoiTextureNode : public EditorNode {
	public:
		VoronoiTextureNode(Point2 position);
	};

	class MusgraveTextureNode : public EditorNode {
	public:
		MusgraveTextureNode(Point2 position);
	};

	class GradientTextureNode : public EditorNode {
	public:
		GradientTextureNode(Point2 position);
	};

	class MagicTextureNode : public EditorNode {
	public:
		MagicTextureNode(Point2 position);
	};

	class CheckerTextureNode : public EditorNode {
	public:
		CheckerTextureNode(Point2 position);
	};

}

