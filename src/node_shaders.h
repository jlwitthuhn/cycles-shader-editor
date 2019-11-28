#pragma once

#include "node_base.h"

namespace cse {

	class FloatPos;

	class PrincipledBSDFNode : public EditableNode {
	public:
		PrincipledBSDFNode(FloatPos position);
	};

	class PrincipledVolumeNode : public EditableNode {
	public:
		PrincipledVolumeNode(FloatPos position);
	};

	class PrincipledHairNode : public EditableNode {
	public:
		PrincipledHairNode(FloatPos position);
	};

	class MixShaderNode : public EditableNode {
	public:
		MixShaderNode(FloatPos position);
	};

	class AddShaderNode : public EditableNode {
	public:
		AddShaderNode(FloatPos position);
	};

	class DiffuseBSDFNode : public EditableNode {
	public:
		DiffuseBSDFNode(FloatPos position);
	};

	class GlossyBSDFNode : public EditableNode {
	public:
		GlossyBSDFNode(FloatPos position);
	};

	class TransparentBSDFNode : public EditableNode {
	public:
		TransparentBSDFNode(FloatPos position);
	};

	class RefractionBSDFNode : public EditableNode {
	public:
		RefractionBSDFNode(FloatPos position);
	};

	class GlassBSDFNode : public EditableNode {
	public:
		GlassBSDFNode(FloatPos position);
	};

	class TranslucentBSDFNode : public EditableNode {
	public:
		TranslucentBSDFNode(FloatPos position);
	};

	class AnisotropicBSDFNode : public EditableNode {
	public:
		AnisotropicBSDFNode(FloatPos position);
	};

	class VelvetBSDFNode : public EditableNode {
	public:
		VelvetBSDFNode(FloatPos position);
	};

	class ToonBSDFNode : public EditableNode {
	public:
		ToonBSDFNode(FloatPos position);
	};

	class SubsurfaceScatteringNode : public EditableNode {
	public:
		SubsurfaceScatteringNode(FloatPos position);
	};

	class EmissionNode : public EditableNode {
	public:
		EmissionNode(FloatPos position);
	};

	class HairBSDFNode : public EditableNode {
	public:
		HairBSDFNode(FloatPos position);
	};

	class HoldoutNode : public EditableNode {
	public:
		HoldoutNode(FloatPos position);
	};

	class VolumeAbsorptionNode : public EditableNode {
	public:
		VolumeAbsorptionNode(FloatPos position);
	};

	class VolumeScatterNode : public EditableNode {
	public:
		VolumeScatterNode(FloatPos position);
	};

}
