#pragma once

#include "node_base.h"

namespace cse {

	class Float2;

	class PrincipledBSDFNode : public EditableNode {
	public:
		PrincipledBSDFNode(Float2 position);
	};

	class PrincipledVolumeNode : public EditableNode {
	public:
		PrincipledVolumeNode(Float2 position);
	};

	class PrincipledHairNode : public EditableNode {
	public:
		PrincipledHairNode(Float2 position);
	};

	class MixShaderNode : public EditableNode {
	public:
		MixShaderNode(Float2 position);
	};

	class AddShaderNode : public EditableNode {
	public:
		AddShaderNode(Float2 position);
	};

	class DiffuseBSDFNode : public EditableNode {
	public:
		DiffuseBSDFNode(Float2 position);
	};

	class GlossyBSDFNode : public EditableNode {
	public:
		GlossyBSDFNode(Float2 position);
	};

	class TransparentBSDFNode : public EditableNode {
	public:
		TransparentBSDFNode(Float2 position);
	};

	class RefractionBSDFNode : public EditableNode {
	public:
		RefractionBSDFNode(Float2 position);
	};

	class GlassBSDFNode : public EditableNode {
	public:
		GlassBSDFNode(Float2 position);
	};

	class TranslucentBSDFNode : public EditableNode {
	public:
		TranslucentBSDFNode(Float2 position);
	};

	class AnisotropicBSDFNode : public EditableNode {
	public:
		AnisotropicBSDFNode(Float2 position);
	};

	class VelvetBSDFNode : public EditableNode {
	public:
		VelvetBSDFNode(Float2 position);
	};

	class ToonBSDFNode : public EditableNode {
	public:
		ToonBSDFNode(Float2 position);
	};

	class SubsurfaceScatteringNode : public EditableNode {
	public:
		SubsurfaceScatteringNode(Float2 position);
	};

	class EmissionNode : public EditableNode {
	public:
		EmissionNode(Float2 position);
	};

	class HairBSDFNode : public EditableNode {
	public:
		HairBSDFNode(Float2 position);
	};

	class HoldoutNode : public EditableNode {
	public:
		HoldoutNode(Float2 position);
	};

	class VolumeAbsorptionNode : public EditableNode {
	public:
		VolumeAbsorptionNode(Float2 position);
	};

	class VolumeScatterNode : public EditableNode {
	public:
		VolumeScatterNode(Float2 position);
	};

}
