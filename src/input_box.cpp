#include "input_box.h"

#include <cctype>
#include <iomanip>
#include <sstream>
#include <stdexcept>

#include <nanovg.h>

#include "gui_sizes.h"
#include "sockets.h"
#include "util_parse.h"

const float INPUT_CORNER_RADIUS = 1.0f;

cse::BaseInputBox::BaseInputBox(const float width, const float height)
{
	this->width = width;
	this->height = height;
}

void cse::BaseInputBox::draw(NVGcontext* const draw_context, const bool hightlight)
{
	// Back fill
	nvgBeginPath(draw_context);
	nvgRoundedRect(draw_context, position.x, position.y, width, height, INPUT_CORNER_RADIUS);
	if (hightlight) {
		nvgFillColor(draw_context, nvgRGBA(230, 230, 230, 255));
	}
	else {
		nvgFillColor(draw_context, nvgRGBA(205, 205, 205, 255));
	}
	nvgFill(draw_context);

	// Font settings
	nvgFontSize(draw_context, UI_FONT_SIZE_NORMAL);
	nvgFontFace(draw_context, "sans");
	nvgTextAlign(draw_context, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
	nvgFontBlur(draw_context, 0.0f);
	nvgFillColor(draw_context, nvgRGBA(0, 0, 0, 255));

	// Socket value
	std::stringstream socket_text_stream;
	if (selected) {
		socket_text_stream << input_stream.str();
	}
	else {
		socket_text_stream << get_value_as_string();
	}
	nvgText(draw_context, position.x + width / 2, position.y + height / 2, socket_text_stream.str().c_str(), nullptr);

	// Outline
	nvgBeginPath(draw_context);
	nvgRoundedRect(draw_context, position.x, position.y, width, height, INPUT_CORNER_RADIUS);
	if (selected) {
		nvgStrokeColor(draw_context, nvgRGBA(255, 255, 255, 255));
	}
	else {
		nvgStrokeColor(draw_context, nvgRGBA(20, 20, 20, 255));
	}
	nvgStrokeWidth(draw_context, 1.0f);
	nvgStroke(draw_context);
}

void cse::BaseInputBox::set_position(const Float2 parent_position)
{
	this->position = parent_position;
}

bool cse::BaseInputBox::contains_point(const Float2 parent_local_pos)
{
	const Float2 local_pos = parent_local_pos - position;

	return (local_pos.x > 0 &&
		local_pos.x < width &&
		local_pos.y > 0 &&
		local_pos.y < height);
}

void cse::BaseInputBox::handle_character(const unsigned int codepoint)
{
	const char as_char = static_cast<char>(codepoint);
	const unsigned char as_uchar = static_cast<unsigned char>(as_char);
	const int is_num = isdigit(as_uchar);
	const int is_punctuation = (as_char == '-' || as_char == '.');

	if (is_num || is_punctuation) {
		input_stream << static_cast<char>(codepoint);
	}
}

void cse::BaseInputBox::backspace()
{
	std::string full_string = input_stream.str();
	if (full_string.size() == 0) {
		return;
	}

	input_stream = std::stringstream();
	input_stream << full_string.substr(0, full_string.size() - 1);
}

void cse::BaseInputBox::begin_edit()
{
	cancel_edit();
	selected = true;
}

bool cse::BaseInputBox::complete_edit()
{
	const std::string user_input = input_stream.str();
	selected = false;
	if (user_input == "") {
		return false;
	}

	const bool result = set_value_from_input_stream();
	input_stream = std::stringstream();
	return result;
}

void cse::BaseInputBox::cancel_edit()
{
	input_stream = std::stringstream();
	selected = false;
}

bool cse::BaseInputBox::should_capture_input() const
{
	return selected;
}

cse::IntInputBox::IntInputBox(const float width, const float height) : BaseInputBox(width, height)
{

}

void cse::IntInputBox::attach_int_value(const std::weak_ptr<IntSocketValue> socket_value_in)
{
	socket_value = socket_value_in;
}

std::string cse::IntInputBox::get_value_as_string()
{
	if (auto socket_value_ptr = socket_value.lock()) {
		std::stringstream value_stream;
		value_stream << socket_value_ptr->get_value();
		return value_stream.str();
	}
	return std::string();
}

bool cse::IntInputBox::set_value_from_input_stream()
{
	if (auto socket_value_ptr = socket_value.lock()) {
		const std::string user_input = input_stream.str();
		try {
			const int old_val = socket_value_ptr->get_value();
			const int new_val = std::stoi(user_input);
			if (old_val != new_val) {
				socket_value_ptr->set_value(new_val);
				return true;
			}
		}
		catch (std::invalid_argument&) {}
		catch (std::out_of_range&) {}
	}
	return false;
}

cse::FloatInputBox::FloatInputBox(const float width, const float height) : BaseInputBox(width, height)
{

}

void cse::FloatInputBox::attach_float_value(const std::weak_ptr<FloatSocketValue> socket_value_in)
{
	socket_value = socket_value_in;
}

void cse::FloatInputBox::set_float_value(const float value)
{
	if (auto socket_value_ptr = socket_value.lock()) {
		socket_value_ptr->set_value(value);
	}
}

float cse::FloatInputBox::get_float_value()
{
	if (auto socket_value_ptr = socket_value.lock()) {
		return socket_value_ptr->get_value();
	}
	return 0.0f;
}

std::string cse::FloatInputBox::get_value_as_string()
{
	if (auto socket_value_ptr = socket_value.lock()) {
		std::stringstream value_stream;
		value_stream << std::fixed << std::setprecision(3) << socket_value_ptr->get_value();
		return value_stream.str();
	}
	return std::string();
}

bool cse::FloatInputBox::set_value_from_input_stream()
{
	if (auto socket_value_ptr = socket_value.lock()) {
		const std::string user_input = input_stream.str();
		try {
			const float old_val = socket_value_ptr->get_value();
			const float new_val = locale_safe_stof(user_input);
			socket_value_ptr->set_value(new_val);
			if (old_val != new_val) {
				socket_value_ptr->set_value(new_val);
				return true;
			}
		}
		catch (std::invalid_argument&) {}
		catch (std::out_of_range&) {}
	}
	return false;
}
