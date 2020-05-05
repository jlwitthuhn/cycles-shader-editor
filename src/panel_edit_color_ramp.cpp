#include "panel_edit_color_ramp.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstring>

#include <GLFW/glfw3.h>
#include <nanovg.h>

#include "drawing.h"
#include "gui_sizes.h"
#include "panel_edit_color.h"
#include "sockets.h"
#include "util_color_ramp.h"
#include "util_enum.h"
#include "util_vector.h"
#include "wrapper_nvg_func.h"

cse::EditColorRampPanel::EditColorRampPanel(const float width) :
	EditParamPanel(width),
	new_point_button(cse::Float2(0.0f, 0.0f), cse::Float2(1.0f, 1.0f)),
	edit_color_panel(std::make_shared<EditColorPanel>(width))
{
	std::memset(preview_tex_color, 0, sizeof(preview_tex_color));
	std::memset(preview_tex_alpha, 0, sizeof(preview_tex_alpha));
}

cse::EditColorRampPanel::~EditColorRampPanel()
{
	if (nvg_handle_preview_color >= 0) {
		nvgDeleteImage(nvg_context, nvg_handle_preview_color);
		nvg_handle_preview_color = -1;
	}
	if (nvg_handle_preview_alpha >= 0) {
		nvgDeleteImage(nvg_context, nvg_handle_preview_alpha);
		nvg_handle_preview_alpha = -1;
	}
}

bool cse::EditColorRampPanel::is_active() const
{
	return attached_ramp.expired() == false;
}

void cse::EditColorRampPanel::pre_draw()
{
	// Remove any rows that have been flagged for deletion
	bool delete_happened;
	do {
		delete_happened = false;
		const auto point_count = ramp_rows.size();
		if (point_count <= 2) {
			// Deleting only works when 3 or more points exist
			break;
		}
		for (auto iter = ramp_rows.begin(); iter != ramp_rows.end(); iter++) {
			if (iter->flagged_for_deletion) {
				ramp_rows.erase(iter);
				delete_happened = true;
				request_undo_push = true;
				break;
			}
		}
	} while (delete_happened);

	// Flip everything back to not requesting deletion
	for (auto& this_row : ramp_rows) {
		this_row.flagged_for_deletion = false;
	}

	// Maybe connect selected color to color panel
	bool color_is_selected = false;
	for (const auto& this_row : ramp_rows) {
		if (this_row.color_selected) {
			edit_color_panel->set_attached_value(this_row.value_color);
			color_is_selected = true;
		}
	}
	if (color_is_selected == false) {
		edit_color_panel->set_attached_value(std::weak_ptr<SocketValue>());
	}

	// Sort all rows by position
	std::sort(
		ramp_rows.begin(),
		ramp_rows.end(),
		[](const ColorRampRow& lhs, const ColorRampRow& rhs) {
			return lhs.value_pos->get_value() < rhs.value_pos->get_value();
		}
	);

	// Copy values from internal types to the real socket
	// FIXME: This doesn't need to happen every frame, may lead to bad performance in extreme cases
	if (const auto ramp_socket_value = attached_ramp.lock()) {
		ramp_socket_value->ramp_points.clear();
		for (const auto& this_row : ramp_rows) {
			const float pos = this_row.value_pos->get_value();
			const Float3 color = this_row.value_color->get_value();
			const float alpha = this_row.value_alpha->get_value();
			const ColorRampPoint new_point(pos, color, alpha);
			ramp_socket_value->ramp_points.push_back(new_point);
		}
	}

	update_preview();

	edit_color_panel->pre_draw();
}

float cse::EditColorRampPanel::draw(NVGcontext* const draw_context)
{
	float height_drawn = 0.0f;

	if (is_active() == false) {
		return height_drawn;
	}

	if (nvg_context == nullptr) {
		nvg_context = draw_context;
		nvg_handle_preview_color = nvgCreateImageRGBA(draw_context, PREVIEW_WIDTH_PX, 1, 0, preview_tex_color);
		nvg_handle_preview_alpha = nvgCreateImageRGBA(draw_context, PREVIEW_WIDTH_PX, 1, 0, preview_tex_alpha);
	}

	// Preview color/alpha strips
	{
		constexpr float PREVIEW_ROW_HPAD = 16.0f;
		constexpr float PREVIEW_ROW_VPAD = 12.0f;
		constexpr float PREVIEW_ROW_BETWEEN = 4.0f;
		constexpr float PREVIEW_COLOR_HEIGHT = 14.0f;
		constexpr float PREVIEW_ALPHA_HEIGHT = 8.0f;
		constexpr float PREVIEW_MARKER_VPAD = 4.0f;
		constexpr float PREVIEW_ROW_HEIGHT = PREVIEW_COLOR_HEIGHT + PREVIEW_ALPHA_HEIGHT + PREVIEW_ROW_VPAD * 2 + PREVIEW_ROW_BETWEEN;

		// Markers
		{
			const float y_lo = height_drawn + PREVIEW_MARKER_VPAD;
			const float y_hi = height_drawn + PREVIEW_ROW_HEIGHT - PREVIEW_MARKER_VPAD;

			const float x_lo = PREVIEW_ROW_HPAD;
			const float x_hi = panel_width - PREVIEW_ROW_HPAD;
			const float x_diff = x_hi - x_lo;

			nvgBeginPath(draw_context);
			nvgStrokeColor(draw_context, nvgRGBf(0.0f, 0.0f, 0.0f));
			nvgStrokeWidth(draw_context, 1.2f);

			for (const ColorRampRow& this_row : ramp_rows) {
				const float this_pos = this_row.value_pos->get_value();
				const float this_x = x_lo + x_diff * this_pos;
				nvgMoveTo(draw_context, this_x, y_lo);
				nvgLineTo(draw_context, this_x, y_hi);
			}

			nvgStroke(draw_context);
		}

		// Color preview
		{
			const Float2 preview_rect_begin(PREVIEW_ROW_HPAD, height_drawn + PREVIEW_ROW_VPAD);
			const Float2 preview_rect_end(panel_width - PREVIEW_ROW_HPAD, preview_rect_begin.y + PREVIEW_COLOR_HEIGHT);
			const Float2 preview_size = preview_rect_end - preview_rect_begin;
			const NVGpaint color_paint = nvgImagePattern(draw_context, preview_rect_begin.x, preview_rect_begin.y, preview_size.x, preview_size.y, 0.0f, nvg_handle_preview_color, 1.0f);

			nvgBeginPath(draw_context);
			nvgRect(draw_context, preview_rect_begin, preview_size);
			nvgFillPaint(draw_context, color_paint);
			nvgFill(draw_context);
		}

		// Alpha preview
		{
			const Float2 alpha_rect_begin(PREVIEW_ROW_HPAD, height_drawn + PREVIEW_ROW_VPAD + PREVIEW_COLOR_HEIGHT + PREVIEW_ROW_BETWEEN);
			const Float2 alpha_rect_end(panel_width - PREVIEW_ROW_HPAD, alpha_rect_begin.y + PREVIEW_ALPHA_HEIGHT);
			const Float2 alpha_size = alpha_rect_end - alpha_rect_begin;
			const NVGpaint alpha_paint = nvgImagePattern(draw_context, alpha_rect_begin.x, alpha_rect_begin.y, alpha_size.x, alpha_size.y, 0.0f, nvg_handle_preview_alpha, 1.0f);

			nvgBeginPath(draw_context);
			nvgRect(draw_context, alpha_rect_begin, alpha_size);
			nvgFillPaint(draw_context, alpha_paint);
			nvgFill(draw_context);
		}

		height_drawn += PREVIEW_ROW_HEIGHT;
	}

	nvgFontSize(draw_context, UI_FONT_SIZE_NORMAL);
	nvgFontFace(draw_context, "sans");
	nvgFontBlur(draw_context, 0.0f);
	nvgFillColor(draw_context, nvgRGBA(0, 0, 0, 255));
	nvgTextAlign(draw_context, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);

	const float point_width = panel_width - UI_SUBWIN_PARAM_EDIT_ROW_HIGHLIGHT_HPAD * 2;

	const float x_left =   UI_SUBWIN_PARAM_EDIT_ROW_HIGHLIGHT_HPAD + point_width / 4.0f;
	const float x_middle = UI_SUBWIN_PARAM_EDIT_ROW_HIGHLIGHT_HPAD + point_width / 2.0f;
	const float x_right =  UI_SUBWIN_PARAM_EDIT_ROW_HIGHLIGHT_HPAD + point_width * 3.0f / 4.0f;

	// Header
	{
		const float y = height_drawn + UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT / 2;

		nvgText(draw_context, x_left, y, "Pos", nullptr);
		nvgText(draw_context, x_middle, y, "Color", nullptr);
		nvgText(draw_context, x_right, y, "Alpha", nullptr);

		height_drawn += UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT;
	}

	// Calculate positions for all buttons
	const float y_box = std::floor((UI_SUBWIN_PARAM_EDIT_COLOR_RAMP_ROW_HEIGHT - UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_HEIGHT) / 2.0f);
	const float x_left_box = std::floor(x_left - (UI_SUBWIN_PARAM_EDIT_COLOR_RAMP_TEXT_INPUT_WIDTH / 2.0f));
	const float x_right_box = std::floor(x_right - (UI_SUBWIN_PARAM_EDIT_COLOR_RAMP_TEXT_INPUT_WIDTH / 2.0f));
	const float x_middle_box = std::floor(x_middle - (UI_SUBWIN_PARAM_EDIT_COLOR_RAMP_TEXT_INPUT_WIDTH / 2.0f));

	for (ColorRampRow& this_row : ramp_rows) {
		const float x_rect = UI_SUBWIN_PARAM_EDIT_ROW_HIGHLIGHT_HPAD;
		const float y_rect = height_drawn + UI_SUBWIN_PARAM_EDIT_ROW_HIGHLIGHT_VPAD;
		const float width = panel_width - 2 * UI_SUBWIN_PARAM_EDIT_ROW_HIGHLIGHT_HPAD;
		const float height = UI_SUBWIN_PARAM_EDIT_COLOR_RAMP_ROW_HEIGHT - 2 * UI_SUBWIN_PARAM_EDIT_ROW_HIGHLIGHT_VPAD;

		// Draw background rectangle for this row
		nvgBeginPath(draw_context);
		nvgRoundedRect(draw_context, x_rect, y_rect, width, height, UI_SUBWIN_PARAM_EDIT_ROW_HIGHLIGHT_CORNER_RADIUS);
		nvgFillColor(draw_context, nvgRGBA(130, 130, 130, 255));
		nvgFill(draw_context);

		// Draw delete button
		const bool allow_delete = (ramp_rows.size() > 2);
		if (allow_delete) {
			constexpr float BUTTON_SIZE = 10.0f;
			constexpr float BUTTON_HPAD = UI_SUBWIN_PARAM_EDIT_ROW_HIGHLIGHT_HPAD;
			const float min_x = UI_SUBWIN_PARAM_EDIT_ROW_HIGHLIGHT_HPAD + BUTTON_HPAD;
			const float max_x = min_x + BUTTON_SIZE;
			const float min_y = height_drawn + (UI_SUBWIN_PARAM_EDIT_COLOR_RAMP_ROW_HEIGHT - BUTTON_SIZE) / 2.0f;
			const float max_y = min_y + BUTTON_SIZE;
			nvgBeginPath(draw_context);
			nvgMoveTo(draw_context, min_x, min_y);
			nvgLineTo(draw_context, max_x, max_y);
			nvgMoveTo(draw_context, max_x, min_y);
			nvgLineTo(draw_context, min_x, max_y);
			nvgStrokeColor(draw_context, nvgRGB(240, 80, 80));
			nvgStrokeWidth(draw_context, 2.0f);
			nvgStroke(draw_context);
			this_row.delete_target = Area(Float2(min_x, min_y), Float2(max_x, max_y));
		}
		else {
			this_row.delete_target = Area(Float2(), Float2());
		}

		// Draw input boxes
		{
			const Float2 left_box_pos(x_left_box, y_box + height_drawn);
			const bool highlight_left = this_row.box_pos.contains_point(mouse_local_pos);
			this_row.box_pos.set_position(left_box_pos);
			this_row.box_pos.draw(draw_context, highlight_left);

			const Float2 right_box_pos(x_right_box, y_box + height_drawn);
			const bool highlight_right = this_row.box_alpha.contains_point(mouse_local_pos);
			this_row.box_alpha.set_position(right_box_pos);
			this_row.box_alpha.draw(draw_context, highlight_right);
		}

		// Draw color swatch
		{
			const Float3 row_color = this_row.value_color->get_value();
			const Float2 swatch_pos(x_middle_box, y_box + height_drawn);
			const float swatch_width = UI_SUBWIN_PARAM_EDIT_COLOR_RAMP_TEXT_INPUT_WIDTH;
			const float swatch_height = UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_HEIGHT;
			Drawing::draw_color_swatch(draw_context, swatch_pos, swatch_width, swatch_height, row_color, this_row.color_selected);
			const Float2 swatch_end(swatch_pos.x + swatch_width, swatch_pos.y + swatch_height);
			this_row.color_target = Area(swatch_pos, swatch_end);
		}

		height_drawn += UI_SUBWIN_PARAM_EDIT_COLOR_RAMP_ROW_HEIGHT;
	}

	// Draw new row button
	{
		const float button_pos_x = 2 * UI_SUBWIN_PARAM_EDIT_ROW_HIGHLIGHT_HPAD;
		const float button_pos_y = height_drawn + UI_SUBWIN_PARAM_EDIT_ROW_HIGHLIGHT_VPAD;
		const Float2 button_pos(button_pos_x, button_pos_y);
		const float button_width = panel_width - 4 * UI_SUBWIN_PARAM_EDIT_ROW_HIGHLIGHT_HPAD;
		const float button_height = UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT - 2 * UI_SUBWIN_PARAM_EDIT_ROW_HIGHLIGHT_VPAD;
		const Float2 button_end(button_pos_x + button_width, button_pos_y + button_height);
		new_point_button = Area(button_pos, button_end);
		Drawing::draw_button(draw_context, button_pos, button_width, button_height, "+", true, new_point_button_pressed);

		height_drawn += UI_SUBWIN_PARAM_EDIT_LAYOUT_ROW_HEIGHT;
	}

	// Draw edit color panel
	if (edit_color_panel->is_active()) {
		edit_color_panel_pos = height_drawn;
		nvgSave(draw_context);
		nvgTranslate(draw_context, 0.0f, height_drawn);
		height_drawn += edit_color_panel->draw(draw_context);
		nvgRestore(draw_context);
	}

	panel_height = height_drawn;
	return panel_height;
}

void cse::EditColorRampPanel::set_mouse_local_position(const Float2 local_pos)
{
	EditParamPanel::set_mouse_local_position(local_pos);

	const Float2 exit_color_mouse_pos(local_pos.x, local_pos.y - edit_color_panel_pos);
	edit_color_panel->set_mouse_local_position(exit_color_mouse_pos);
}

bool cse::EditColorRampPanel::has_input_focus() const
{
	bool result = false;
	for (const auto& this_row : ramp_rows) {
		result = this_row.box_pos.has_input_focus() || result;
		result = this_row.box_alpha.has_input_focus() || result;
	}
	result = edit_color_panel->has_input_focus() || result;
	return result && is_active();
}

void cse::EditColorRampPanel::handle_mouse_button(const int button, const int action, const int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		deselect_input_box();
		for (auto& this_row : ramp_rows) {
			if (this_row.box_pos.contains_point(mouse_local_pos)) {
				this_row.box_pos.begin_edit();
			}
			if (this_row.box_alpha.contains_point(mouse_local_pos)) {
				this_row.box_alpha.begin_edit();
			}
			if (this_row.color_target.contains_point(mouse_local_pos)) {
				this_row.color_selected = true;
			}
			if (this_row.delete_target.contains_point(mouse_local_pos)) {
				this_row.flagged_for_deletion = true;
			}
		}
		if (new_point_button.contains_point(mouse_local_pos)) {
			new_point_button_pressed = true;
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		if (new_point_button.contains_point(mouse_local_pos) && new_point_button_pressed) {
			ColorRampPoint new_point(1.0f, Float3(1.0f, 1.0f, 1.0f), 1.0f);
			ramp_rows.push_back(new_point);
		}
		new_point_button_pressed = false;
	}

	edit_color_panel->handle_mouse_button(button, action, mods);
}

void cse::EditColorRampPanel::handle_key(const int key, const int scancode, const int action, const int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		for (auto& this_row : ramp_rows) {
			this_row.box_pos.cancel_edit();
			this_row.box_alpha.cancel_edit();
		}
	}
	else if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
		for (auto& this_row : ramp_rows) {
			request_undo_push = this_row.box_pos.complete_edit() || request_undo_push;
			request_undo_push = this_row.box_alpha.complete_edit() || request_undo_push;
		}
	}
	else if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS) {
		for (auto& this_row : ramp_rows) {
			this_row.box_pos.backspace();
			this_row.box_alpha.backspace();
		}
	}

	if (edit_color_panel->has_input_focus()) {
		edit_color_panel->handle_key(key, scancode, action, mods);
	}
}

void cse::EditColorRampPanel::handle_character(const unsigned int codepoint)
{
	for (auto& this_row : ramp_rows) {
		if (this_row.has_input_focus()) {
			this_row.handle_character(codepoint);
		}
	}

	if (edit_color_panel->has_input_focus()) {
		edit_color_panel->handle_character(codepoint);
	}
}

void cse::EditColorRampPanel::set_attached_value(const std::weak_ptr<SocketValue> socket_value)
{
	if (auto socket_value_ptr = socket_value.lock()) {
		if (socket_value_ptr->get_type() == SocketType::COLOR_RAMP) {
			const auto ramp_value_ptr = std::dynamic_pointer_cast<ColorRampSocketValue>(socket_value_ptr);
			if (attached_ramp.lock() != ramp_value_ptr) {
				attached_ramp = ramp_value_ptr;
				ramp_rows.clear();
				for (const auto& this_point : ramp_value_ptr->ramp_points) {
					ramp_rows.push_back(this_point);
				}
			}
			return;
		}
	}
	attached_ramp = std::weak_ptr<ColorRampSocketValue>();
	ramp_rows.clear();
}

void cse::EditColorRampPanel::deselect_input_box()
{
	for (auto& this_row : ramp_rows) {
		request_undo_push = this_row.box_pos.complete_edit() || request_undo_push;
		request_undo_push = this_row.box_alpha.complete_edit() || request_undo_push;
		if (edit_color_panel->is_mouse_over() == false) {
			this_row.color_selected = false;
		}
	}
}

cse::EditColorRampPanel::ColorRampRow::ColorRampRow(const cse::ColorRampPoint point) :
	box_pos(UI_SUBWIN_PARAM_EDIT_COLOR_RAMP_TEXT_INPUT_WIDTH, UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_HEIGHT),
	box_alpha(UI_SUBWIN_PARAM_EDIT_COLOR_RAMP_TEXT_INPUT_WIDTH, UI_SUBWIN_PARAM_EDIT_TEXT_INPUT_HEIGHT),
	color_target(Float2(0.0f, 0.0f), Float2(0.0f, 0.0f)),
	delete_target(Float2(0.0f, 0.0f), Float2(0.0f, 0.0f)),
	value_pos(std::make_shared<FloatSocketValue>(point.position, 0.0f, 1.0f)),
	value_alpha(std::make_shared<FloatSocketValue>(point.alpha, 0.0f, 1.0f)),
	value_color(std::make_shared<ColorSocketValue>(point.color.x, point.color.y, point.color.z))
{
	box_pos.attach_float_value(value_pos);
	box_alpha.attach_float_value(value_alpha);
}

bool cse::EditColorRampPanel::ColorRampRow::has_input_focus() const
{
	bool result = false;
	result = box_pos.has_input_focus() || result;
	result = box_alpha.has_input_focus() || result;
	return result;
}

void cse::EditColorRampPanel::ColorRampRow::handle_character(const unsigned int codepoint)
{
	if (box_pos.has_input_focus()) {
		box_pos.handle_character(codepoint);
	}
	if (box_alpha.has_input_focus()) {
		box_alpha.handle_character(codepoint);
	}
}

bool cse::EditColorRampPanel::should_push_undo_state()
{
	bool result = false;
	result = EditParamPanel::should_push_undo_state() || result;
	result = edit_color_panel->should_push_undo_state() || result;
	return result;
}

void cse::EditColorRampPanel::tab()
{
	if (something_is_selected()) {
		bool select_next{ false };
		for (ColorRampRow& this_row : ramp_rows) {
			if (select_next) {
				this_row.box_pos.begin_edit();
				break;
			}
			if (this_row.box_pos.has_input_focus()) {
				this_row.box_pos.complete_edit();
				this_row.color_selected = true;
			}
			else if (this_row.color_selected) {
				edit_color_panel->tab();
				if (edit_color_panel->has_input_focus() == false) {
					// We have tabbed past color
					this_row.color_selected = false;
					this_row.box_alpha.begin_edit();
				}
			}
			else if (this_row.box_alpha.has_input_focus()) {
				this_row.box_alpha.complete_edit();
				select_next = true;
			}
		}
	}
	else {
		for (ColorRampRow& this_row : ramp_rows) {
			this_row.box_pos.begin_edit();
			break;
		}
	}
}

void cse::EditColorRampPanel::update_preview()
{
	if (nvg_context == nullptr) {
		// Texture has not been created yet, nothing to update
		return;
	}

	static_assert(PREVIEW_CHANNELS >= 4, "Expected at least 4 channels in preview texture");

	if (const auto ramp = attached_ramp.lock()) {
		const auto samples = ramp->evaluate_samples(PREVIEW_WIDTH_PX);
		assert(samples.size() == PREVIEW_WIDTH_PX);

		for (auto i = 0; i < PREVIEW_WIDTH_PX; i++) {
			const auto index = i * PREVIEW_CHANNELS;
			const Float4& this_sample = samples[i];
			preview_tex_color[index + 0] = static_cast<unsigned char>(this_sample.x * 255.0);
			preview_tex_color[index + 1] = static_cast<unsigned char>(this_sample.y * 255.0);
			preview_tex_color[index + 2] = static_cast<unsigned char>(this_sample.z * 255.0);
			preview_tex_color[index + 3] = 255;
		}
		nvgUpdateImage(nvg_context, nvg_handle_preview_color, preview_tex_color);

		for (auto i = 0; i < PREVIEW_WIDTH_PX; i++) {
			const auto index = i * PREVIEW_CHANNELS;
			const Float4& this_sample = samples[i];
			preview_tex_alpha[index + 0] = static_cast<unsigned char>(this_sample.w * 255.0);
			preview_tex_alpha[index + 1] = static_cast<unsigned char>(this_sample.w * 255.0);
			preview_tex_alpha[index + 2] = static_cast<unsigned char>(this_sample.w * 255.0);
			preview_tex_alpha[index + 3] = 255;
		}
		nvgUpdateImage(nvg_context, nvg_handle_preview_alpha, preview_tex_alpha);
	}
}

bool cse::EditColorRampPanel::something_is_selected() const
{
	bool result{ false };
	for (const ColorRampRow& this_row : ramp_rows) {
		result = result || this_row.box_pos.has_input_focus();
		result = result || this_row.color_selected;
		result = result || this_row.box_alpha.has_input_focus();
	}
	return result;
}