#pragma once

#include <map>
#include <string>
#include <vector>

namespace cse {

	enum class CyclesNodeType {
		// Shader
		AmbientOcclusion,
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
		LightPath,
		Fresnel,
		LayerWeight,
		CameraData,
		Tangent,
		TextureCoordinate,
		Geometry,
		ObjectInfo,
		RGB,
		Wireframe,
		Value,
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

	class Float2 {
	public:
		Float2();
		Float2(float x, float y);

		float x, y;
	};

	class Float3 {
	public:
		Float3();
		Float3(float x, float y, float z);

		float x, y, z;
	};

	struct OutputCurve {
		std::vector<Float2> control_points;
		int enum_curve_interp = 0;
		std::vector<float> samples;
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
	};

	struct OutputConnection {
		std::string source_node;
		std::string source_socket;
		std::string dest_node;
		std::string dest_socket;
	};

}
