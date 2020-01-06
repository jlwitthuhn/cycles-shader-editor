#include "button_category.h"

#include <cmath>

#include "drawing.h"

cse::NodeCategoryButtonPlacer::NodeCategoryButtonPlacer(const Float2 draw_origin, const float parent_width, const float vertical_padding)
{
	this->draw_origin = draw_origin;
	this->parent_width = parent_width;
	this->vertical_padding = vertical_padding;

	this->button_width = NodeCategoryButton::get_button_width();
	this->button_height = NodeCategoryButton::get_button_height();

	positions_made = 0;
}

cse::Float2 cse::NodeCategoryButtonPlacer::next_button_position()
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

	const Float2 result(x, y);
	return draw_origin + result;
}

float cse::NodeCategoryButtonPlacer::get_draw_height()
{
	const int row_count = (positions_made + 1) / 2;

	return row_count * (2 * vertical_padding + button_height);
}

cse::NodeCategoryButton::NodeCategoryButton(const std::string& label)
{
	this->label = label;
}

void cse::NodeCategoryButton::draw(const Float2 draw_position, NVGcontext* const draw_context)
{
	constexpr bool BUTTON_ENABLED = true;
	Drawing::draw_button(draw_context, draw_position, get_button_width(), get_button_height(), label, BUTTON_ENABLED, selected);
}

void cse::NodeCategoryButton::update_mouse_position(const Float2 local_position)
{
	mouse_local_pos = local_position;
}

bool cse::NodeCategoryButton::is_mouse_over_button() const
{
	return (
		mouse_local_pos.x > 0.0f &&
		mouse_local_pos.x < get_button_width() &&
		mouse_local_pos.y > 0.0f &&
		mouse_local_pos.y < get_button_height()
		);
}
