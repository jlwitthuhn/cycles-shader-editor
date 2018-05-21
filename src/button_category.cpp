#include "button_category.h"

#include <cmath>

#include <nanovg.h>

#include "buttons_nodes.h"
#include "drawing.h"
#include "gui_sizes.h"

CyclesShaderEditor::NodeCategoryButtonPlacer::NodeCategoryButtonPlacer(const Point2 draw_origin, const float parent_width, const float vertical_padding)
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

	const int rows_complete = positions_made / 2;
	const float y = rows_complete * (2 * vertical_padding + button_height) + vertical_padding;

	++positions_made;

	const Point2 result(x, y);
	return draw_origin + result;
}

float CyclesShaderEditor::NodeCategoryButtonPlacer::get_draw_height()
{
	const int row_count = (positions_made + 1) / 2;

	return row_count * (2 * vertical_padding + button_height);
}

CyclesShaderEditor::NodeCategoryButton::NodeCategoryButton(const std::string& label)
{
	this->label = label;
}

CyclesShaderEditor::NodeCategoryButton::~NodeCategoryButton()
{
	for (NodeCreationButton* const node_button : node_buttons) {
		delete node_button;
	}
}

void CyclesShaderEditor::NodeCategoryButton::draw(const Point2 draw_position, NVGcontext* const draw_context)
{
	Drawing::draw_button(draw_context, draw_position.get_pos_x(), draw_position.get_pos_y(), get_button_width(), get_button_height(), label, true, pressed);
}

void CyclesShaderEditor::NodeCategoryButton::update_mouse_position(const Point2 local_position)
{
	mouse_local_pos = local_position;
}

bool CyclesShaderEditor::NodeCategoryButton::is_mouse_over_button()
{
	return (
		mouse_local_pos.get_pos_x() > 0.0f &&
		mouse_local_pos.get_pos_x() < get_button_width() &&
		mouse_local_pos.get_pos_y() > 0.0f &&
		mouse_local_pos.get_pos_y() < get_button_height()
		);
}