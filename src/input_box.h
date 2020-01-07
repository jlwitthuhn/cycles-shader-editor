#pragma once

#include <memory>
#include <string>

#include "util_vector.h"

struct NVGcontext;

namespace cse {

	class FloatSocketValue;
	class IntSocketValue;

	class BaseInputBox {
	public:
		BaseInputBox(float width, float height);
		virtual ~BaseInputBox() {}

		void draw(NVGcontext* draw_context, bool highlight);
		void set_position(Float2 parent_position);
		bool contains_point(Float2 parent_local_pos);

		void handle_character(unsigned int codepoint);
		void backspace();

		void begin_edit();
		bool complete_edit();
		void cancel_edit();

		bool should_capture_input() const;

		float width;
		float height;

	protected:
		virtual std::string get_value_as_string() = 0;
		virtual bool set_value_from_input_string() = 0;

		// Position of this UI element, relative to parent component
		Float2 position;

		bool selected = false;

		std::string input_string;
	};

	class IntInputBox : public BaseInputBox {
	public:
		IntInputBox(float width, float height);
		virtual ~IntInputBox() override {}

		void attach_int_value(std::weak_ptr<IntSocketValue> socket_value);

	private:
		virtual std::string get_value_as_string() override;
		virtual bool set_value_from_input_string() override;

		std::weak_ptr<IntSocketValue> socket_value;
	};

	class FloatInputBox : public BaseInputBox {
	public:
		FloatInputBox(float width, float height);
		virtual ~FloatInputBox() override {}

		void attach_float_value(std::weak_ptr<FloatSocketValue> socket_value);
		void set_float_value(float value);
		float get_float_value();

	private:
		virtual std::string get_value_as_string() override;
		virtual bool set_value_from_input_string() override;

		std::weak_ptr<FloatSocketValue> socket_value;
	};

}
