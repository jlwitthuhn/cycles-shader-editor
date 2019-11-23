#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "float_pos.h"
#include "input_box.h"
#include "sockets.h"

namespace cse {
	// Widget that can accept input for multiple socket values
	class MultiInputWidget {
	public:
		MultiInputWidget(float width);

		bool add_socket_input(std::string label, std::weak_ptr<SocketValue> socket_value);
		void clear_sockets();

		bool complete_input();

		float draw(NVGcontext* draw_context);

		void set_mouse_local_position(FloatPos local_pos);

		bool should_capture_input() const;
		void handle_mouse_button(int button, int action, int mods);
		void handle_key(int key, int scancode, int action, int mods);
		void handle_character(unsigned int codepoint);

		bool should_push_undo_state();

	private:
		const float width;

		class InputRow {
		public:
			InputRow(std::string label, std::weak_ptr<SocketValue> socket_value, std::shared_ptr<cse::BaseInputBox> input_box);

			const std::string label;
			const std::weak_ptr<SocketValue> socket_value;
			const std::shared_ptr<cse::BaseInputBox> input_box;
		};

		std::vector<InputRow> sockets;

		FloatPos mouse_local_pos;

		bool request_undo_push = false;
	};
}
