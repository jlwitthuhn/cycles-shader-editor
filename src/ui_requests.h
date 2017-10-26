#pragma once

namespace CyclesShaderEditor {

	class UIRequests {
	public:
		bool save = false;
		bool undo = false;
		bool redo = false;

		bool move_left = false;
		bool move_right = false;
		bool move_up = false;
		bool move_down = false;

		bool zoom_in = false;
		bool zoom_out = false;
	};

}
