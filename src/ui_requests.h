#pragma once

namespace CyclesShaderEditor {

	// Every frame, requests from several UI components will be combined with operator|=
	// For each flag that is true, the main window will take some action
	class ViewUIRequests {
	public:
		ViewUIRequests();

		void clear();

		void operator|=(const ViewUIRequests& other);

		bool pan_left = false;
		bool pan_right = false;
		bool pan_up = false;
		bool pan_down = false;

		bool zoom_in = false;
		bool zoom_out = false;
	};
	
	class UIRequests {
	public:
		UIRequests();

		void clear();

		void operator|=(const UIRequests& other);

		bool save = false;
		bool undo = false;
		bool redo = false;

		ViewUIRequests view;
	};

}
