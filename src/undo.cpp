#include "undo.h"

static const int UNDO_LIMIT = 50;

void cse::UndoStack::push_undo_state(std::string state)
{
	redo_state.clear();
	undo_state.push_front(state);
	while (undo_state.size() > UNDO_LIMIT) {
		undo_state.pop_back();
	}
}

std::string cse::UndoStack::pop_undo_state(std::string current_state)
{
	if (undo_state.size() == 0) {
		return current_state;
	}
	redo_state.push_front(current_state);
	std::string result_state = undo_state.front();
	undo_state.pop_front();
	return result_state;
}

std::string cse::UndoStack::pop_redo_state(std::string current_state)
{
	if (redo_state.size() == 0) {
		return current_state;
	}
	undo_state.push_front(current_state);
	std::string result_state = redo_state.front();
	redo_state.pop_front();
	return result_state;
}

bool cse::UndoStack::undo_available()
{
	return (undo_state.size() > 0);
}

bool cse::UndoStack::redo_available()
{
	return (redo_state.size() > 0);
}

void cse::UndoStack::clear()
{
	undo_state.clear();
	redo_state.clear();
}
