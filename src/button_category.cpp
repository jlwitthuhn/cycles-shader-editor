#include "button_category.h"

#include <cmath>

#include <nanovg.h>

#include "buttons_nodes.h"
#include "gui_sizes.h"

CyclesShaderEditor::NodeCategoryButtonPlacer::NodeCategoryButtonPlacer(Point2 draw_origin, float parent_width, float vertical_padding)
{
	this->draw_origin = draw_origin;
	this->parent_width = parent_width;
	this->vertical_padding = vertical_padding;

	this->button_width = NodeCategoryButton::get_button_width();
	this->button_height = NodeCategoryButton::get_button_height();

	positions_made = 0;
}

CyclesShaderEditor::Point2 CyclesShaderEditor::NodeCategoryButtonPlacer::next_button_position()
{
	// Find horizontal position
	float x;
	const float total_content_width = button_width * 2 + UI_SUBWIN_NODE_LIST_CAT_BUTTON_HGAP;
	const float hpadding = std::floor((parent_width - total_content_width) / 2.0f);
	// Left button
	if (positions_made % 2 == 0) {
		x = hpadding;
	}
	// Right button
	else {
		x = parent_width - hpadding - button_width;
	}

	int rows_complete = positions_made / 2;
	float y = rows_complete * (2 * vertical_padding + button_height) + vertical_padding;

	++positions_made;

	Point2 result(x, y);
	return draw_origin + result;
}

float CyclesShaderEditor::NodeCategoryButtonPlacer::get_draw_height()
{
	int row_count = (positions_made + 1) / 2;

	return row_count * (2 * vertical_padding + button_height);
}

CyclesShaderEditor::NodeCategoryButton::NodeCategoryButton(std::string label)
{
	this->label = label;
}

CyclesShaderEditor::NodeCategoryButton::~NodeCategoryButton()
{
	for (NodeCreationButton* node_button : node_buttons) {
		delete node_button;
	}
}

void CyclesShaderEditor::NodeCategoryButton::draw(Point2 draw_position, NVGcontext* draw_context)
{
	nvgSave(draw_context);

	nvgTranslate(draw_context, draw_position.get_pos_x(), draw_position.get_pos_y());

	NVGpaint button_bg;
	if (pressed) {
		button_bg = nvgLinearGradient(draw_context,
			0.0f,
			0.0f,
			0.0f,
			get_button_height(),
			nvgRGBA(0, 0, 0, 32),
			nvgRGBA(255, 255, 255, 32));
	}
	else {
		button_bg = nvgLinearGradient(draw_context,
			0.0f,
			0.0f,
			0.0f,
			get_button_height(),
			nvgRGBA(255, 255, 255, 32),
			nvgRGBA(0, 0, 0, 32));
	}

	// Button shape
	nvgBeginPath(draw_context);
	nvgRoundedRect(draw_context, 0.0f, 0.0f, get_button_width(), get_button_height(), UI_BUTTON_CORNER_RADIUS);

	// Fill in gradient
	nvgFillPaint(draw_context, button_bg);
	nvgFill(draw_context);

	// Outline
	nvgStrokeWidth(draw_context, 1.0f);
	nvgStrokeColor(draw_context, nvgRGBAf(0.0f, 0.0f, 0.0f, 0.5f));
	nvgStroke(draw_context);

	// Label
	nvgFontSize(draw_context, UI_FONT_SIZE_NORMAL);
	nvgFontFace(draw_context, "sans");
	nvgTextAlign(draw_context, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
	nvgFontBlur(draw_context, 0.0f);
	nvgFillColor(draw_context, nvgRGBA(0, 0, 0, 255));
	nvgText(draw_context, get_button_width() / 2, get_button_height() / 2, label.c_str(), NULL);

	nvgRestore(draw_context);
}

void CyclesShaderEditor::NodeCategoryButton::update_mouse_position(Point2 local_position)
{
	mouse_local_pos = local_position;
}

bool CyclesShaderEditor::NodeCategoryButton::is_mouse_over_button()
{
	return (
		mouse_local_pos.get_pos_x() > 0.0f &&
		mouse_local_pos.get_pos_x() < get_button_width() &&
		mouse_local_pos.get_pos_y() > 0.0f &&
		mouse_local_pos.get_pos_y() < get_button_height());
}