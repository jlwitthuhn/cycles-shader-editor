#pragma once

#include <vector>

namespace CyclesShaderEditor {

	class ZoomManager {
	public:
		ZoomManager();

		float get_world_scale();

		void zoom_in();
		void zoom_out();

	private:
		int zoom_level;
		std::vector<float> zooms;
	};

}