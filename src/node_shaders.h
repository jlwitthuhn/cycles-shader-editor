#pragma once

#include "node_base.h"

namespace cse {

	class FloatPos;

	class AmbientOcculsionNode : public EditorNode {
	public:
		AmbientOcculsionNode(FloatPos position);
	};

	class PrincipledBSDFNode : public EditorNode {
	public:
		PrincipledBSDFNode(FloatPos position);
	};

	class MixShaderNode : public EditorNode {
	public:
		MixShaderNode(FloatPos position);
	};

	class AddShaderNode : public EditorNode {
	public:
		AddShaderNode(FloatPos position);
	};

	class DiffuseBSDFNode : public EditorNode {
	public:
		DiffuseBSDFNode(FloatPos position);
	};

	class GlossyBSDFNode : public EditorNode {
	public:
		GlossyBSDFNode(FloatPos position);
	};

	class TransparentBSDFNode : public EditorNode {
	public:
		TransparentBSDFNode(FloatPos position);
	};

	class RefractionBSDFNode : public EditorNode {
	public:
		RefractionBSDFNode(FloatPos position);
	};

	class GlassBSDFNode : public EditorNode {
	public:
		GlassBSDFNode(FloatPos position);
	};

	class TranslucentBSDFNode : public EditorNode {
	public:
		TranslucentBSDFNode(FloatPos position);
	};

	class AnisotropicBSDFNode : public EditorNode {
	public:
		AnisotropicBSDFNode(FloatPos position);
	};

	class VelvetBSDFNode : public EditorNode {
	public:
		VelvetBSDFNode(FloatPos position);
	};

	class ToonBSDFNode : public EditorNode {
	public:
		ToonBSDFNode(FloatPos position);
	};

	class SubsurfaceScatteringNode : public EditorNode {
	public:
		SubsurfaceScatteringNode(FloatPos position);
	};

	class EmissionNode : public EditorNode {
	public:
		EmissionNode(FloatPos position);
	};

	class HairBSDFNode : public EditorNode {
	public:
		HairBSDFNode(FloatPos position);
	};

	class HoldoutNode : public EditorNode {
	public:
		HoldoutNode(FloatPos position);
	};

	class VolumeAbsorptionNode : public EditorNode {
	public:
		VolumeAbsorptionNode(FloatPos position);
	};

	class VolumeScatterNode : public EditorNode {
	public:
		VolumeScatterNode(FloatPos position);
	};

}
