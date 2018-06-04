#pragma once

namespace CyclesShaderEditor {

	// Every frame, requests from several UI components will be combined with operator|=
	// For each flag that is true, the main window will take some action
	class UIRequests {
	public:
		UIRequests();

		void clear();

		void operator|=(const UIRequests& other);

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
