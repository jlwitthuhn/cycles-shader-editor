#include "input_box.h"

#include <cctype>
#include <cstring>
#include <iomanip>
#include <sstream>

#include <nanovg.h>

#include "node_base.h"

const float INPUT_CORNER_RADIUS = 3.0f;

CyclesShaderEditor::BaseInputBox::BaseInputBox(const float width, const float height)
{
	this->width = width;
	this->height = height;
}

void CyclesShaderEditor::BaseInputBox::draw(NVGcontext* const draw_context, const FloatPos parent_mouse_pos)
{
	this->parent_mouse_pos = parent_mouse_pos;

	// Back fill
	nvgBeginPath(draw_context);
	nvgRoundedRect(draw_context, position.get_pos_x(), position.get_pos_y(), width, height, INPUT_CORNER_RADIUS);
	if (is_mouse_over()) {
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
	nvgText(draw_context, position.get_pos_x() + width / 2, position.get_pos_y() + height / 2, socket_text_stream.str().c_str(), nullptr);

	if (selected) {
		// Outline
		nvgBeginPath(draw_context);
		nvgRoundedRect(draw_context, position.get_pos_x(), position.get_pos_y(), width, height, INPUT_CORNER_RADIUS);
		nvgStrokeColor(draw_context, nvgRGBA(0, 0, 0, 255));
		nvgStrokeWidth(draw_context, 1.0f);
		nvgStroke(draw_context);
	}
}

void CyclesShaderEditor::BaseInputBox::set_position(const FloatPos parent_position)
{
	this->position = parent_position;
}

bool CyclesShaderEditor::BaseInputBox::is_mouse_over()
{
	if (displayed == false) {
		return false;
	}

	return (parent_mouse_pos.get_pos_x() > position.get_pos_x() &&
		parent_mouse_pos.get_pos_x() < position.get_pos_x() + width &&
		parent_mouse_pos.get_pos_y() > position.get_pos_y() &&
		parent_mouse_pos.get_pos_y() < position.get_pos_y() + height);
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

bool CyclesShaderEditor::BaseInputBox::should_capture_keys()
{
	return selected;
}

CyclesShaderEditor::IntInputBox::IntInputBox(const float width, const float height) : BaseInputBox(width, height)
{

}

void CyclesShaderEditor::IntInputBox::set_int_value(IntSocketValue* const socket_value)
{
	this->socket_value = socket_value;
}

std::string CyclesShaderEditor::IntInputBox::get_value_as_string()
{
	if (socket_value == nullptr) {
		return std::string();
	}

	std::stringstream value_stream;
	value_stream << socket_value->get_value();
	return value_stream.str();
}

void CyclesShaderEditor::IntInputBox::set_value_from_input_stream()
{
	if (socket_value == nullptr) {
		return;
	}

	const std::string user_input = input_stream.str();

	try {
		int val = std::stoi(user_input);
		socket_value->set_value(val);
	}
	catch (std::invalid_argument&) { }
	catch (std::out_of_range&) { }
}

CyclesShaderEditor::FloatInputBox::FloatInputBox(float width, float height) : BaseInputBox(width, height)
{

}

void CyclesShaderEditor::FloatInputBox::set_float_value(FloatSocketValue* socket_value)
{
	this->socket_value = socket_value;
}

void CyclesShaderEditor::FloatInputBox::set_float_value(float value)
{
	if (socket_value == nullptr) {
		return;
	}
	socket_value->set_value(value);
}

float CyclesShaderEditor::FloatInputBox::get_float_value()
{
	if (socket_value == nullptr) {
		return 0.0f;
	}

	return socket_value->get_value();
}

std::string CyclesShaderEditor::FloatInputBox::get_value_as_string()
{
	if (socket_value == nullptr) {
		return std::string();
	}

	std::stringstream value_stream;
	value_stream << std::fixed << std::setprecision(3) << socket_value->get_value();
	return value_stream.str();
}

void CyclesShaderEditor::FloatInputBox::set_value_from_input_stream()
{
	if (socket_value == nullptr) {
		return;
	}

	const std::string user_input = input_stream.str();

	try {
		float val = std::stof(user_input);
		socket_value->set_value(val);
	}
	catch (std::invalid_argument&) { }
	catch (std::out_of_range&) { }
}
