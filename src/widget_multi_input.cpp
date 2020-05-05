#include "widget_multi_input.h"

#include <GLFW/glfw3.h>
#include <nanovg.h>

#include "gui_sizes.h"
#include "input_box.h"
#include "sockets.h"

static std::shared_ptr<cse::BaseInputBox> get_input_box_for_value(const std::shared_ptr<cse::SocketValue>& socket_value)
{
	if (const auto as_int = std::dynamic_pointer_cast<cse::IntSocketValue>(socket_value)) {
		const auto result = std::make_shared<cse::IntInputBox>(UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_WIDTH, UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_HEIGHT);
		result->attach_int_value(as_int);
		return result;
	}
	else if (const auto as_float = std::dynamic_pointer_cast<cse::FloatSocketValue>(socket_value)) {
		const auto result = std::make_shared<cse::FloatInputBox>(UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_WIDTH, UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_HEIGHT);
		result->attach_float_value(as_float);
		return result;
	}
	else {
		// Unsupported type, return empty ptr
		return std::shared_ptr<cse::BaseInputBox>();
	}
}

cse::MultiInputWidget::MultiInputWidget(const float width) : Widget(width)
{

}

bool cse::MultiInputWidget::tab()
{
	const bool something_selected{ has_input_focus() };
	if (something_selected) {
		// Advance to the next box
		bool select_next{ false };
		for (const InputRow& socket : sockets) {
			if (select_next) {
				socket.input_box->begin_edit();
				return has_input_focus();
			}
			if (socket.input_box->has_input_focus()) {
				socket.input_box->complete_edit();
				select_next = true;
			}
		}
		// If we reach this line, this means the selected box was last
		// In that case, keep nothing selected
	}
	else {
		// Select the first box
		for (const InputRow& socket : sockets) {
			socket.input_box->begin_edit();
			break;
		}
	}
	return has_input_focus();
}

bool cse::MultiInputWidget::add_socket_input(const std::string label, const std::weak_ptr<SocketValue> socket_value)
{
	if (const auto locked = socket_value.lock()) {
		const std::shared_ptr<cse::BaseInputBox> input_box = get_input_box_for_value(locked);
		if (input_box) {
			sockets.push_back(InputRow(label, socket_value, input_box));
			return true;
		}
	}
	return false;
}

void cse::MultiInputWidget::clear_sockets()
{
	sockets.clear();
}

bool cse::MultiInputWidget::complete_input()
{
	bool value_has_changed = false;
	for (const auto& this_socket : sockets) {
		value_has_changed = this_socket.input_box->complete_edit() || value_has_changed;
	}
	return value_has_changed;
}

float cse::MultiInputWidget::draw(NVGcontext* const draw_context)
{
	float height_drawn = 0.0f;

	if (sockets.size() == 0) {
		// No sockets exist, show error
		nvgFontSize(draw_context, UI_FONT_SIZE_NORMAL);
		nvgFontFace(draw_context, "sans");
		nvgTextAlign(draw_context, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
		nvgFontBlur(draw_context, 0.0f);
		nvgFillColor(draw_context, nvgRGBA(0, 0, 0, 255));

		const std::string value_text = "Error: No sockets to display";
		nvgText(draw_context, width / 2, height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT / 2, value_text.c_str(), nullptr);

		height_drawn += UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT;
	}
	else {
		// Iterate through and draw all sockets
		for (auto this_socket : sockets) {
			if (this_socket.socket_value.expired()) {
				continue;
			}

			const std::shared_ptr<SocketValue> this_val = this_socket.socket_value.lock();

			nvgFontSize(draw_context, UI_FONT_SIZE_NORMAL);
			nvgFontFace(draw_context, "sans");
			nvgTextAlign(draw_context, NVG_ALIGN_RIGHT | NVG_ALIGN_MIDDLE);
			nvgFontBlur(draw_context, 0.0f);
			nvgFillColor(draw_context, nvgRGBA(0, 0, 0, 255));

			const float text_x_draw = width - this_socket.input_box->width - 3 * UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_HPAD;
			const float text_y_draw = height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT / 2;
			nvgText(draw_context, text_x_draw, text_y_draw, this_socket.label.c_str(), nullptr);

			const float input_x_draw = width - this_socket.input_box->width - 2 * UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_HPAD;
			const float input_y_draw = height_drawn + (UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT - this_socket.input_box->height) / 2;
			const bool highlight = this_socket.input_box->contains_point(mouse_pos);
			this_socket.input_box->set_position(Float2(input_x_draw, input_y_draw));
			this_socket.input_box->draw(draw_context, highlight);

			height_drawn += UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT;
		}
	}

	return height_drawn;
}

bool cse::MultiInputWidget::has_input_focus() const
{
	bool result = false;
	for (const auto& this_socket : sockets) {
		result = this_socket.input_box->has_input_focus() || result;
	}
	return result;
}

void cse::MultiInputWidget::handle_mouse_button(const int button, const int action, int)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		for (const auto& this_socket : sockets) {
			if (this_socket.input_box->contains_point(mouse_pos)) {
				this_socket.input_box->begin_edit();
			}
			else {
				request_undo_push = this_socket.input_box->complete_edit() || request_undo_push;
			}
		}
	}
}

void cse::MultiInputWidget::handle_key(const int key, int, const int action, int)
{
	for (const auto& this_socket : sockets) {
		if (this_socket.input_box->has_input_focus()) {
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
				this_socket.input_box->cancel_edit();
			}
			else if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
				request_undo_push = this_socket.input_box->complete_edit() || request_undo_push;
			}
			else if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS) {
				this_socket.input_box->backspace();
			}
		}
	}
}

void cse::MultiInputWidget::handle_character(const unsigned int codepoint)
{
	for (const auto& this_socket : sockets) {
		if (this_socket.input_box->has_input_focus()) {
			this_socket.input_box->handle_character(codepoint);
		}
	}
}

cse::MultiInputWidget::InputRow::InputRow(const std::string label, const std::weak_ptr<SocketValue> socket_value, const std::shared_ptr<cse::BaseInputBox> input_box) :
	label(label),
	socket_value(socket_value),
	input_box(input_box)
{

}
