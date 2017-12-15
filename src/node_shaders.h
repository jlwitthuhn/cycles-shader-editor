#pragma once

#include "node_base.h"

namespace CyclesShaderEditor {

	class AmbientOcculsionNode : public EditorNode {
	public:
		AmbientOcculsionNode(Point2 position);
	};

	class PrincipledBSDFNode : public EditorNode {
	public:
		PrincipledBSDFNode(Point2 position);
	};

	class MixShaderNode : public EditorNode {
	public:
		MixShaderNode(Point2 position);
	};

	class AddShaderNode : public EditorNode {
	public:
		AddShaderNode(Point2 position);
	};

	class DiffuseBSDFNode : public EditorNode {
	public:
		DiffuseBSDFNode(Point2 position);
	};

	class GlossyBSDFNode : public EditorNode {
	public:
		GlossyBSDFNode(Point2 position);
	};

	class TransparentBSDFNode : public EditorNode {
	public:
		TransparentBSDFNode(Point2 position);
	};

	class RefractionBSDFNode : public EditorNode {
	public:
		RefractionBSDFNode(Point2 position);
	};

	class GlassBSDFNode : public EditorNode {
	public:
		GlassBSDFNode(Point2 position);
	};

	class TranslucentBSDFNode : public EditorNode {
	public:
		TranslucentBSDFNode(Point2 position);
	};

	class AnisotropicBSDFNode : public EditorNode {
	public:
		AnisotropicBSDFNode(Point2 position);
	};

	class VelvetBSDFNode : public EditorNode {
	public:
		VelvetBSDFNode(Point2 position);
	};

	class ToonBSDFNode : public EditorNode {
	public:
		ToonBSDFNode(Point2 position);
	};

	class SubsurfaceScatteringNode : public EditorNode {
	public:
		SubsurfaceScatteringNode(Point2 position);
	};

	class EmissionNode : public EditorNode {
	public:
		EmissionNode(Point2 position);
	};

	class HairBSDFNode : public EditorNode {
	public:
		HairBSDFNode(Point2 position);
	};

	class HoldoutNode : public EditorNode {
	public:
		HoldoutNode(Point2 position);
	};

	class VolumeAbsorptionNode : public EditorNode {
	public:
		VolumeAbsorptionNode(Point2 position);
	};

	class VolumeScatterNode : public EditorNode {
	public:
		VolumeScatterNode(Point2 position);
	};

}
