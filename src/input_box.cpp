#include "input_box.h"

#include <cctype>
#include <iomanip>
#include <sstream>
#include <stdexcept>

#include <nanovg.h>

#include "sockets.h"
#include "util_parse.h"

const float INPUT_CORNER_RADIUS = 3.0f;

CyclesShaderEditor::BaseInputBox::BaseInputBox(const float width, const float height)
{
	this->width = width;
	this->height = height;
}

void CyclesShaderEditor::BaseInputBox::draw(NVGcontext* const draw_context, const bool hightlight)
{
	// Back fill
	nvgBeginPath(draw_context);
	nvgRoundedRect(draw_context, position.get_x(), position.get_y(), width, height, INPUT_CORNER_RADIUS);
	if (hightlight) {
		nvgFillColor(draw_context, nvgRGBA(235, 235, 235, 255));
	}
	else {
		nvgFillColor(draw_context, nvgRGBA(225, 225, 225, 255));
	}
	nvgFill(draw_context);

	// Font settings
	nvgFontSize(draw_context, 18.0f);
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
	nvgText(draw_context, position.get_x() + width / 2, position.get_y() + height / 2, socket_text_stream.str().c_str(), nullptr);

	if (selected) {
		// Outline
		nvgBeginPath(draw_context);
		nvgRoundedRect(draw_context, position.get_x(), position.get_y(), width, height, INPUT_CORNER_RADIUS);
		nvgStrokeColor(draw_context, nvgRGBA(0, 0, 0, 255));
		nvgStrokeWidth(draw_context, 1.0f);
		nvgStroke(draw_context);
	}
}

void CyclesShaderEditor::BaseInputBox::set_position(const FloatPos parent_position)
{
	this->position = parent_position;
}

bool CyclesShaderEditor::BaseInputBox::is_under_point(const FloatPos parent_local_pos)
{
	if (displayed == false) {
		return false;
	}

	const FloatPos local_pos = parent_local_pos - position;

	return (local_pos.get_x() > 0 &&
		local_pos.get_x() < width &&
		local_pos.get_y() > 0 &&
		local_pos.get_y() < height);
}

void CyclesShaderEditor::BaseInputBox::handle_character(const unsigned int codepoint)
{
	const char as_char = static_cast<char>(codepoint);
	const unsigned char as_uchar = static_cast<unsigned char>(as_char);
	const int is_num = isdigit(as_uchar);
	const int is_punctuation = (as_char == '-' || as_char == '.');

	if (is_num || is_punctuation) {
		input_stream << static_cast<char>(codepoint);
	}
}

void CyclesShaderEditor::BaseInputBox::backspace()
{
	std::string full_string = input_stream.str();
	if (full_string.size() == 0) {
		return;
	}

	input_stream = std::stringstream();
	input_stream << full_string.substr(0, full_string.size() - 1);
}

void CyclesShaderEditor::BaseInputBox::begin_edit()
{
	input_stream = std::stringstream();
	selected = true;
}

void CyclesShaderEditor::BaseInputBox::complete_edit()
{
	const std::string user_input = input_stream.str();
	selected = false;
	if (user_input == "") {
		return;
	}

	set_value_from_input_stream();
}

void CyclesShaderEditor::BaseInputBox::cancel_edit()
{
	input_stream = std::stringstream();
	selected = false;
}

bool CyclesShaderEditor::BaseInputBox::should_capture_input() const
{
	return selected;
}

CyclesShaderEditor::IntInputBox::IntInputBox(const float width, const float height) : BaseInputBox(width, height)
{

}

void CyclesShaderEditor::IntInputBox::set_int_value(const std::weak_ptr<IntSocketValue> socket_value_in)
{
	socket_value = socket_value_in;
}

std::string CyclesShaderEditor::IntInputBox::get_value_as_string()
{
	if (auto socket_value_ptr = socket_value.lock()) {
		std::stringstream value_stream;
		value_stream << socket_value_ptr->get_value();
		return value_stream.str();
	}
	return std::string();
}

void CyclesShaderEditor::IntInputBox::set_value_from_input_stream()
{
	if (auto socket_value_ptr = socket_value.lock()) {
		const std::string user_input = input_stream.str();
		try {
			int val = std::stoi(user_input);
			socket_value_ptr->set_value(val);
		}
		catch (std::invalid_argument&) {}
		catch (std::out_of_range&) {}
	}
}

CyclesShaderEditor::FloatInputBox::FloatInputBox(const float width, const float height) : BaseInputBox(width, height)
{

}

void CyclesShaderEditor::FloatInputBox::set_float_value(const std::weak_ptr<FloatSocketValue> socket_value_in)
{
	socket_value = socket_value_in;
}

void CyclesShaderEditor::FloatInputBox::set_float_value(const float value)
{
	if (auto socket_value_ptr = socket_value.lock()) {
		socket_value_ptr->set_value(value);
	}
}

float CyclesShaderEditor::FloatInputBox::get_float_value()
{
	if (auto socket_value_ptr = socket_value.lock()) {
		return socket_value_ptr->get_value();
	}
	return 0.0f;
}

std::string CyclesShaderEditor::FloatInputBox::get_value_as_string()
{
	if (auto socket_value_ptr = socket_value.lock()) {
		std::stringstream value_stream;
		value_stream << std::fixed << std::setprecision(3) << socket_value_ptr->get_value();
		return value_stream.str();
	}
	return std::string();
}

void CyclesShaderEditor::FloatInputBox::set_value_from_input_stream()
{
	if (auto socket_value_ptr = socket_value.lock()) {
		const std::string user_input = input_stream.str();

		try {
			float val = locale_safe_stof(user_input);
			socket_value_ptr->set_value(val);
		}
		catch (std::invalid_argument&) {}
		catch (std::out_of_range&) {}
	}
}
