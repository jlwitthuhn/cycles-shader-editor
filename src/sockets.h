#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "util_color_ramp.h"
#include "util_enum.h"
#include "util_vector.h"

namespace cse {

	class EditableNode;

	class SocketValue {
	public:
		virtual ~SocketValue() {}
		virtual SocketType get_type() const = 0;
	};

	class IntSocketValue : public SocketValue {
	public:
		IntSocketValue(int default_val, int min, int max);

		virtual SocketType get_type() const override;

		int get_value();
		void set_value(int value_in);

	private:
		int value;

		int default_val;
		int min;
		int max;
	};

	class FloatSocketValue : public SocketValue {
	public:
		FloatSocketValue(float default_val, float min, float max);

		virtual SocketType get_type() const override;

		float get_value();
		void set_value(float value_in);

	private:
		float value;

		float default_val;
		float min;
		float max;
	};

	class Float3SocketValue : public SocketValue {
	public:
		Float3SocketValue(float default_x, float min_x, float max_x,
			float default_y, float min_y, float max_y,
			float default_z, float min_z, float max_z);

		virtual SocketType get_type() const override;

		Float3 get_value();
		void set_x(float x_in);
		void set_y(float y_in);
		void set_z(float z_in);

		std::shared_ptr<FloatSocketValue> x_socket_val;
		std::shared_ptr<FloatSocketValue> y_socket_val;
		std::shared_ptr<FloatSocketValue> z_socket_val;
	};

	class ColorSocketValue : public SocketValue {
	public:
		ColorSocketValue(float default_r, float default_g, float default_b);

		virtual SocketType get_type() const override;

		Float3 get_value();

		std::shared_ptr<FloatSocketValue> r_socket_val;
		std::shared_ptr<FloatSocketValue> g_socket_val;
		std::shared_ptr<FloatSocketValue> b_socket_val;

		float last_hue = 0.0f;
	};

	class StringEnumPair {
	public:
		StringEnumPair(const std::string& display_value, const std::string& internal_value);

		std::string display_value;
		std::string internal_value;
	};

	class StringEnumSocketValue : public SocketValue {
	public:
		StringEnumSocketValue();

		virtual SocketType get_type() const override;

		bool set_from_internal_name(std::string internal_name);

		StringEnumPair value;
		std::vector<StringEnumPair> enum_values;
	};

	class BoolSocketValue : public SocketValue {
	public:
		BoolSocketValue(bool default_val);

		virtual SocketType get_type() const override;

		bool value;
		bool default_value;
	};

	class CurveSocketValue : public SocketValue {
	public:
		CurveSocketValue();

		virtual SocketType get_type() const override;

		void reset_value();
		void create_point(float x);
		void delete_point(const Float2& target);
		// Gets the index, if any, of the closest point within selection range
		bool get_target_index(const Float2& target, std::size_t& index);
		// Move a point and return its new index
		std::size_t move_point(std::size_t index, const Float2& new_point);

		void sort_curve_points();

		std::vector<Float2> curve_points;
		CurveInterpolation curve_interp = CurveInterpolation::CUBIC_HERMITE;
	};

	class ColorRampSocketValue : public SocketValue {
	public:
		ColorRampSocketValue();

		virtual SocketType get_type() const override;

		std::vector<ColorRampPoint> ramp_points;

		std::vector<Float4> evaluate_samples() const;
	};

	class NodeSocket {
	public:
		NodeSocket(EditableNode* parent, SocketIOType io_type, SocketType socket_type, std::string display_name, std::string internal_name);

		void set_float_val(float float_in);
		void set_float3_val(float x_in, float y_in, float z_in);
		void set_string_val(StringEnumPair string_in);

		EditableNode* const parent;

		SocketIOType io_type;
		SocketType socket_type;
		std::string display_name;
		std::string internal_name;

		bool draw_socket = true;

		bool selectable = false;

		// World position where this socket was drawn last frame
		Float2 world_draw_position;

		std::shared_ptr<SocketValue> value;

		// This variable is used to track when a value should be crossed out in the UI
		// It should be set to true prior to calling draw() when this socket's input has a connection
		// It will be reset to false when the parent node's draw() is called
		bool input_connected_this_frame = false;
	};

}
