#pragma once

#include <memory>
#include <sstream>
#include <string>

#include "float_pos.h"

struct NVGcontext;

namespace cse {

	class FloatSocketValue;
	class IntSocketValue;

	class BaseInputBox {
	public:
		BaseInputBox(float width, float height);

		void draw(NVGcontext* draw_context, bool highlight);
		void set_position(FloatPos parent_position);
		bool is_under_point(FloatPos parent_local_pos);

		void handle_character(unsigned int codepoint);
		void backspace();

		void begin_edit();
		void complete_edit();
		void cancel_edit();

		bool should_capture_input() const;

		float width;
		float height;

		bool displayed = false;

	protected:
		virtual std::string get_value_as_string() = 0;
		virtual void set_value_from_input_stream() = 0;

		// Position of this UI element, relative to parent component
		FloatPos position;

		bool selected = false;

		std::stringstream input_stream;
	};

	class IntInputBox : public BaseInputBox {
	public:
		IntInputBox(float width, float height);

		void set_int_value(std::weak_ptr<IntSocketValue> socket_value);

	private:
		virtual std::string get_value_as_string();
		virtual void set_value_from_input_stream();

		std::weak_ptr<IntSocketValue> socket_value;
	};

	class FloatInputBox : public BaseInputBox {
	public:
		FloatInputBox(float width, float height);

		void set_float_value(std::weak_ptr<FloatSocketValue> socket_value);
		void set_float_value(float value);
		float get_float_value();

	private:
		virtual std::string get_value_as_string();
		virtual void set_value_from_input_stream();

		std::weak_ptr<FloatSocketValue> socket_value;
	};

}
