#pragma once

#include <sstream>
#include <string>

#include "float_pos.h"

struct NVGcontext;

namespace CyclesShaderEditor {

	class FloatSocketValue;
	class IntSocketValue;

	class BaseInputBox {
	public:
		BaseInputBox(float width, float height);

		void draw(NVGcontext* draw_context, FloatPos parent_mouse_pos);
		void set_position(FloatPos parent_position);
		bool is_mouse_over();

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

		// Position of this UI element, relative to parent subwindow
		FloatPos position;

		// Position of mouse, relative to parent
		FloatPos parent_mouse_pos;

		bool selected = false;

		std::stringstream input_stream;
	};

	class IntInputBox : public BaseInputBox {
	public:
		IntInputBox(float width, float height);

		void set_int_value(IntSocketValue* socket_value);

	private:
		virtual std::string get_value_as_string();
		virtual void set_value_from_input_stream();

		IntSocketValue* socket_value = nullptr;
	};

	class FloatInputBox : public BaseInputBox {
	public:
		FloatInputBox(float width, float height);

		void set_float_value(FloatSocketValue* socket_value);
		void set_float_value(float value);
		float get_float_value();

	private:
		virtual std::string get_value_as_string();
		virtual void set_value_from_input_stream();

		FloatSocketValue* socket_value = nullptr;
	};

}