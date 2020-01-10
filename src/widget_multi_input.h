#pragma once

#include <memory>
#include <string>
#include <vector>

#include "util_vector.h"
#include "widget.h"

struct NVGcontext;

namespace cse {

	class BaseInputBox;
	class SocketValue;

	// Widget that can accept input for multiple socket values
	class MultiInputWidget : public Widget {
	public:
		MultiInputWidget(float width);

		bool add_socket_input(std::string label, std::weak_ptr<SocketValue> socket_value);
		void clear_sockets();

		bool complete_input();

		virtual float draw(NVGcontext* draw_context) override;

		virtual bool has_input_focus() const override;
		virtual void handle_mouse_button(int button, int action, int mods) override;
		virtual void handle_key(int key, int scancode, int action, int mods) override;
		virtual void handle_character(unsigned int codepoint) override;

	private:
		class InputRow {
		public:
			InputRow(std::string label, std::weak_ptr<SocketValue> socket_value, std::shared_ptr<cse::BaseInputBox> input_box);

			const std::string label;
			const std::weak_ptr<SocketValue> socket_value;
			const std::shared_ptr<cse::BaseInputBox> input_box;
		};

		std::vector<InputRow> sockets;
	};
}
