#pragma once

#include <map>
#include <string>

namespace CyclesShaderEditor {

	enum class CyclesNodeType {
		// Shader
		PrincipledBSDF,
		MixShader,
		AddShader,
		DiffuseBSDF,
		GlossyBSDF,
		TransparentBSDF,
		RefractionBSDF,
		GlassBSDF,
		TranslucentBSDF,
		AnisotropicBSDF,
		VelvetBSDF,
		ToonBSDF,
		SubsurfaceScattering,
		Emission,
		HairBSDF,
		Holdout,
		VolAbsorption,
		VolScatter,
		// Texture
		NoiseTex,
		WaveTex,
		VoronoiTex,
		MusgraveTex,
		GradientTex,
		MagicTex,
		CheckerTex,
		MaxTex,
		// Input
		LightPath,
		Fresnel,
		LayerWeight,
		CameraData,
		Tangent,
		TextureCoordinate,
		// Color
		MixRGB,
		Invert,
		LightFalloff,
		HSV,
		Gamma,
		BrightnessContrast,
		// Vector
		Bump,
		NormalMap,
		// Output
		MaterialOutput,
		Unknown,
		// Other
		Count,
	};

	struct Float3 {
		float x, y, z;
	};

	struct OutputNode {
		CyclesNodeType type;

		std::string name;

		float world_x;
		float world_y;

		std::map<std::string, float> float_values;
		std::map<std::string, Float3> float3_values;
		std::map<std::string, std::string> string_values;
		std::map<std::string, int> int_values;
		std::map<std::string, bool> bool_values;
	};

	struct OutputConnection {
		std::string source_node;
		std::string source_socket;
		std::string dest_node;
		std::string dest_socket;
	};

}
