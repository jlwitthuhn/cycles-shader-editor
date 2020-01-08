#include "widget_radio_list.h"

#include <cmath>

#include <GLFW/glfw3.h>
#include <nanovg.h>

#include "gui_sizes.h"
#include "sockets.h"

cse::RadioListWidget::RadioListWidget(const float width) : width(width)
{

}

void cse::RadioListWidget::attach_value(const std::weak_ptr<BoolSocketValue> bool_socket_value)
{
	string_pairs.clear();
	this->attached_enum = std::weak_ptr<StringEnumSocketValue>();
	if (auto locked = bool_socket_value.lock()) {
		this->attached_bool = locked;
		string_pairs.push_back(StringEnumPair("True", "true"));
		string_pairs.push_back(StringEnumPair("False", "tralse"));
	}
}

void cse::RadioListWidget::attach_value(const std::weak_ptr<StringEnumSocketValue> enum_socket_value)
{
	string_pairs.clear();
	this->attached_bool = std::weak_ptr<BoolSocketValue>();
	if (auto locked = enum_socket_value.lock()) {
		this->attached_enum = locked;
		for (const StringEnumPair& this_pair : locked->enum_values) {
			string_pairs.push_back(this_pair);
		}
	}
}

void cse::RadioListWidget::pre_draw()
{
	click_areas.clear();
}

float cse::RadioListWidget::draw(NVGcontext* const draw_context)
{
	float height_drawn = 0.0f;

	if (attached_enum.use_count() == 0 && attached_bool.use_count() == 0) {
		return height_drawn;
	}

	nvgFontSize(draw_context, UI_FONT_SIZE_NORMAL);
	nvgFontFace(draw_context, "sans");
	nvgFontBlur(draw_context, 0.0f);
	nvgFillColor(draw_context, nvgRGBA(0, 0, 0, 255));
	nvgTextAlign(draw_context, NVG_ALIGN_LEFT | NVG_ALIGN_BASELINE);

	float max_label_width = 0;
	for (StringEnumPair this_enum_value : string_pairs) {
		float bounds[4];
		nvgTextBounds(draw_context, 0, 0, this_enum_value.display_value.c_str(), nullptr, bounds);
		float this_width = std::ceil(bounds[2]);
		if (this_width > max_label_width) {
			max_label_width = this_width;
		}
	}
	const float max_draw_width = max_label_width + UI_CHECKBOX_SPACING + UI_CHECKBOX_RADIUS * 2;

	const auto enum_ptr = attached_enum.lock();
	const auto bool_ptr = attached_bool.lock();
	for (StringEnumPair this_enum_value : string_pairs) {
		bool this_value_selected = false;
		if (enum_ptr) {
			this_value_selected = (enum_ptr->value.internal_value == this_enum_value.internal_value);
		}
		else if (bool_ptr) {
			const bool this_bool = this_enum_value.internal_value == "true";
			this_value_selected = (this_bool == bool_ptr->value);
		}

		float circle_pos_x = width / 2.0f - max_draw_width / 2 + UI_CHECKBOX_RADIUS;
		float circle_pos_y = height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT * 0.5f;
		nvgBeginPath(draw_context);
		nvgCircle(draw_context, circle_pos_x, circle_pos_y, UI_CHECKBOX_RADIUS);
		nvgFillColor(draw_context, nvgRGBAf(0.1f, 0.1f, 0.1f, 1.0f));
		nvgFill(draw_context);
		if (this_value_selected) {
			nvgBeginPath(draw_context);
			nvgCircle(draw_context, circle_pos_x, circle_pos_y, UI_CHECKBOX_RADIUS * 0.666f);
			nvgFillColor(draw_context, nvgRGBAf(0.9f, 0.9f, 0.9f, 1.0f));
			nvgFill(draw_context);
		}

		float text_pos_x = width / 2.0f - max_draw_width / 2 + UI_CHECKBOX_SPACING + UI_CHECKBOX_RADIUS * 2;
		float text_pos_y = height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT * 0.7f;
		nvgFillColor(draw_context, nvgRGBA(0, 0, 0, 255));
		nvgText(draw_context, text_pos_x, text_pos_y, this_enum_value.display_value.c_str(), nullptr);

		Float2 click_area_begin(0.0f, height_drawn);
		Float2 click_area_end(width, height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT);
		HolderArea<std::string> click_area(click_area_begin, click_area_end, this_enum_value.internal_value);

		click_areas.push_back(click_area);

		height_drawn += UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT;
	}

	return height_drawn;
}

void cse::RadioListWidget::set_mouse_local_position(const Float2 local_pos)
{
	mouse_local_pos = local_pos;
}

void cse::RadioListWidget::handle_mouse_button(const int button, const int action, const int /*mods*/)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		for (const HolderArea<std::string>& this_area : click_areas) {
			if (this_area.contains_point(mouse_local_pos)) {
				if (auto enum_value = attached_enum.lock()) {
					if (enum_value->value.internal_value != this_area.get_value()) {
						enum_value->set_from_internal_name(this_area.get_value());
						request_undo_push = true;
					}
				}
				else if (auto bool_value = attached_bool.lock()) {
					const bool new_value = (this_area.get_value() == "true") ? true : false;
					if (bool_value->value != new_value) {
						bool_value->value = new_value;
						request_undo_push = true;
					}
				}
			}
		}
	}
}

bool cse::RadioListWidget::should_push_undo_state()
{
	const bool result = request_undo_push;
	request_undo_push = false;
	return result;
}
