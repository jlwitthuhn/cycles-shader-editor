#pragma once

#include <map>
#include <string>
#include <vector>

#include "util_vector.h"

namespace cse {

	enum class CyclesNodeType {
		// Shader
		PrincipledBSDF,
		PrincipledVolume,
		PrincipledHair,
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
		MaxTex,
		BrickTex,
		CheckerTex,
		GradientTex,
		MagicTex,
		MusgraveTex,
		NoiseTex,
		VoronoiTex,
		WaveTex,
		// Input
		AmbientOcclusion,
		Bevel,
		CameraData,
		Fresnel,
		Geometry,
		LayerWeight,
		LightPath,
		ObjectInfo,
		RGB,
		Tangent,
		TextureCoordinate,
		Value,
		Wireframe,
		// Color
		MixRGB,
		Invert,
		LightFalloff,
		HSV,
		Gamma,
		BrightnessContrast,
		RGBCurves,
		// Vector
		Bump,
		Displacement,
		NormalMap,
		VectorTransform,
		// Converter
		Blackbody,
		ColorRamp,
		CombineHSV,
		CombineRGB,
		CombineXYZ,
		Math,
		RGBtoBW,
		SeparateHSV,
		SeparateRGB,
		SeparateXYZ,
		VectorMath,
		Wavelength,
		// Output
		MaterialOutput,
		Unknown,
		// Other
		Count,
	};

	class Float4 {
	public:
		Float4(float x, float y, float z, float w);

		float x, y, z, w;
	};

	struct OutputCurve {
		std::vector<Float2> control_points;
		int enum_curve_interp = 0;
		std::vector<float> samples;
	};

	struct OutputColorRampPoint {
		float pos;
		Float3 color;
		float alpha;
	};

	struct OutputColorRamp {
		std::vector<OutputColorRampPoint> points;
		std::vector<Float3> samples_color;
		std::vector<float> samples_alpha;
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
		std::map<std::string, OutputCurve> curve_values;
		std::map<std::string, OutputColorRamp> ramp_values;
	};

	struct OutputConnection {
		std::string source_node;
		std::string source_socket;
		std::string dest_node;
		std::string dest_socket;
	};

}
