#include "statusbar.h"

#include <nanovg.h>


CyclesShaderEditor::NodeEditorStatusBar::NodeEditorStatusBar()
{
	status_text = "Node graph editor loaded";
}

void CyclesShaderEditor::NodeEditorStatusBar::draw(NVGcontext* const draw_context, const float width)
{
	// Panel
	nvgBeginPath(draw_context);
	nvgRoundedRect(draw_context, 0.0f, 0.0f, width, get_status_bar_height(), 0.0f);
	nvgFillColor(draw_context, nvgRGBA(180, 180, 180, 255));
	nvgFill(draw_context);

	// Status text
	nvgFontSize(draw_context, UI_FONT_SIZE_NORMAL);
	nvgFontFace(draw_context, "sans");
	nvgTextAlign(draw_context, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
	nvgFontBlur(draw_context, 0.0f);
	nvgFillColor(draw_context, nvgRGBA(0, 0, 0, 255));
	nvgText(draw_context, 4.0f, get_status_bar_height() / 2.0f, status_text.c_str(), NULL);

	// Zoom text
	nvgFontSize(draw_context, UI_FONT_SIZE_NORMAL);
	nvgFontFace(draw_context, "sans");
	nvgTextAlign(draw_context, NVG_ALIGN_RIGHT | NVG_ALIGN_MIDDLE);
	nvgFontBlur(draw_context, 0.0f);
	nvgFillColor(draw_context, nvgRGBA(0, 0, 0, 255));
	nvgText(draw_context, width - 4.0f, get_status_bar_height() / 2.0f, zoom_text.c_str(), NULL);
}

void CyclesShaderEditor::NodeEditorStatusBar::set_status_text(const std::string& text)
{
	status_text = text;
}

void CyclesShaderEditor::NodeEditorStatusBar::set_zoom_text(const std::string& text)
{
	zoom_text = text;
}