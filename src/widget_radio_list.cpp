#include "widget_radio_list.h"

#include <nanovg.h>

#include "gui_sizes.h"
#include "sockets.h"

cse::RadioListWidget::RadioListWidget(const float width) : width(width)
{

}

void cse::RadioListWidget::attach_enum(const std::weak_ptr<StringEnumSocketValue> enum_socket_value)
{
	this->enum_socket_value = enum_socket_value;
}

float cse::RadioListWidget::draw(NVGcontext* const draw_context)
{
	const auto attached_enum = enum_socket_value.lock();

	float height_drawn = 0.0f;

	nvgFontSize(draw_context, UI_FONT_SIZE_NORMAL);
	nvgFontFace(draw_context, "sans");
	nvgFontBlur(draw_context, 0.0f);
	nvgFillColor(draw_context, nvgRGBA(0, 0, 0, 255));
	nvgTextAlign(draw_context, NVG_ALIGN_LEFT | NVG_ALIGN_BASELINE);

	float max_label_width = 0;
	for (StringEnumPair this_enum_value : attached_enum->enum_values) {
		float bounds[4];
		nvgTextBounds(draw_context, 0, 0, this_enum_value.display_value.c_str(), nullptr, bounds);
		float this_width = std::ceil(bounds[2]);
		if (this_width > max_label_width) {
			max_label_width = this_width;
		}
	}
	const float max_draw_width = max_label_width + UI_CHECKBOX_SPACING + UI_CHECKBOX_RADIUS * 2;

	for (StringEnumPair this_enum_value : attached_enum->enum_values) {
		const bool this_value_selected = (attached_enum->value.internal_value == this_enum_value.internal_value);

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

		// TODO:
		FloatPos click_area_begin(0.0f, height_drawn);
		FloatPos click_area_end(width, height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT);
		StringEnumArea click_area(click_area_begin, click_area_end, this_enum_value, attached_enum);

		enum_targets.push_back(click_area);

		height_drawn += UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT;
	}

	return height_drawn;
}

void cse::RadioListWidget::set_mouse_local_position(const FloatPos local_pos)
{
	mouse_local_pos = local_pos;
}

void cse::RadioListWidget::handle_mouse_button(const int, const int, const int)
{
	for (StringEnumArea& this_target : enum_targets) {
		if (this_target.is_under_point(mouse_local_pos)) {
			request_undo_push = this_target.click() || request_undo_push;
		}
	}
}

bool cse::RadioListWidget::should_push_undo_state()
{
	const bool result = request_undo_push;
	request_undo_push = false;
	return result;
}
