#pragma once

#include <list>
#include <string>

namespace cse {

	class UndoStack {
	public:
		void push_undo_state(std::string state);

		std::string pop_undo_state(std::string current_state);
		std::string pop_redo_state(std::string current_state);

		bool undo_available();
		bool redo_available();

		void clear();

	private:
		std::list<std::string> undo_state;
		std::list<std::string> redo_state;
	};

}
