#include "zoom.h"

#include <cassert>

CyclesShaderEditor::ZoomManager::ZoomManager()
{
	zooms.push_back(0.20f);
	zooms.push_back(0.25f);
	zooms.push_back(0.325f);
	zooms.push_back(0.40f);
	zooms.push_back(0.50f);
	zooms.push_back(0.666667f);
	zooms.push_back(0.833333f);
	zooms.push_back(1.00f);
	zooms.push_back(1.25f);
	zooms.push_back(1.50f);
	zooms.push_back(1.75f);
	zooms.push_back(2.00f);
	zooms.push_back(2.333333f);
	zooms.push_back(2.666667f);
	zooms.push_back(3.00f);
	zooms.push_back(3.50f);
	zooms.push_back(4.00f);
	zooms.push_back(5.00f);

	zoom_level = -1;
	for (size_t i = 0; i < zooms.size(); i++) {
		if (zooms[i] == 1.0f) {
			zoom_level = static_cast<int>(i);
		}
	}

	if (zoom_level < 0) {
		assert(false);
	}
}

float CyclesShaderEditor::ZoomManager::get_world_scale()
{
	return zooms[zoom_level];
}

void CyclesShaderEditor::ZoomManager::zoom_in()
{
	if (zoom_level == static_cast<int>(zooms.size() - 1)) {
		return;
	}
	zoom_level++;
}

void CyclesShaderEditor::ZoomManager::zoom_out()
{
	if (zoom_level == 0) {
		return;
	}
	zoom_level--;
}