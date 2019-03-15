#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "common_enums.h"
#include "float_pos.h"

namespace cse {

	class EditorNode;

	struct Float3Holder {
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	};

	struct FloatRGBColor {
		float r = 1.0f;
		float g = 1.0f;
		float b = 1.0f;
	};

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

		Float3Holder get_value();
		void set_x(float x_in);
		void set_y(float x_in);
		void set_z(float x_in);

		std::shared_ptr<FloatSocketValue> x_socket_val;
		std::shared_ptr<FloatSocketValue> y_socket_val;
		std::shared_ptr<FloatSocketValue> z_socket_val;
	};

	class ColorSocketValue : public SocketValue {
	public:
		ColorSocketValue(float default_r, float default_g, float default_b);

		virtual SocketType get_type() const override;

		FloatRGBColor get_value();

		std::shared_ptr<FloatSocketValue> red_socket_val;
		std::shared_ptr<FloatSocketValue> green_socket_val;
		std::shared_ptr<FloatSocketValue> blue_socket_val;
	};

	class StringEnumPair {
	public:
		StringEnumPair(std::string display_value, std::string internal_value);

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
		void delete_point(const FloatPos& target);
		// Gets the index, if any, of the closest point within selection range
		bool get_target_index(const FloatPos& target, std::size_t& index);
		// Move a point and return its new index
		std::size_t move_point(std::size_t index, const FloatPos& new_point);

		void sort_curve_points();

		std::vector<FloatPos> curve_points;
		CurveInterpolation curve_interp = CurveInterpolation::CUBIC_HERMITE;
	};

	class NodeSocket {
	public:
		NodeSocket(EditorNode* parent, SocketIOType io_type, SocketType socket_type, std::string display_name, std::string internal_name);

		void set_float_val(float float_in);
		void set_float3_val(float x_in, float y_in, float z_in);
		void set_string_val(StringEnumPair string_in);

		EditorNode* parent = nullptr;

		SocketIOType io_type;
		SocketType socket_type;
		std::string display_name;
		std::string internal_name;

		bool draw_socket = true;

		bool selectable = false;

		// World position where this socket was drawn last frame
		FloatPos world_draw_position;

		std::shared_ptr<SocketValue> value;

		// This variable is used to track when a value should be crossed out in the UI
		// It should be set to true prior to calling draw() when this socket's input has a connection
		// It will be reset to false when the parent node's draw() is called
		bool input_connected_this_frame = false;
	};

}
