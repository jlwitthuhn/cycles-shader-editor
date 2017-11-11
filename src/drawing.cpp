#include "drawing.h"

#include <nanovg.h>

#include "gui_sizes.h"

void CyclesShaderEditor::Drawing::draw_button(NVGcontext* draw_context, float x_pos, float y_pos, float width, float height, std::string label, bool enabled, bool pressed)
{
	const int gradient_alpha = enabled ? 31 : 15;
	const int button_alpha = enabled ? 127 : 63;
	const int label_alpha = enabled ? 255 : 127;

	const unsigned char start_color = pressed ? 0 : 255;
	const unsigned char end_color = pressed ? 127 : 15;

	NVGpaint button_bg = nvgLinearGradient(
		draw_context,
		x_pos,
		y_pos,
		x_pos,
		y_pos + height,
		nvgRGBA(start_color, start_color, start_color, gradient_alpha),
		nvgRGBA(end_color, end_color, end_color, gradient_alpha)
	);

	// Draw button
	nvgBeginPath(draw_context);
	nvgRoundedRect(draw_context,
		x_pos,
		y_pos,
		width,
		height,
		UI_BUTTON_CORNER_RADIUS);
	nvgFillPaint(draw_context, button_bg);
	nvgFill(draw_context);
	nvgStrokeColor(draw_context, nvgRGBA(0, 0, 0, button_alpha));
	nvgStrokeWidth(draw_context, 1.0f);
	nvgStroke(draw_context);

	// Button label
	nvgFontSize(draw_context, UI_FONT_SIZE_NORMAL);
	nvgFontFace(draw_context, "sans");
	nvgTextAlign(draw_context, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
	nvgFontBlur(draw_context, 0.0f);
	nvgFillColor(draw_context, nvgRGBA(0, 0, 0, label_alpha));
	nvgText(draw_context, x_pos + width / 2, y_pos + height / 2, label.c_str(), NULL);
}