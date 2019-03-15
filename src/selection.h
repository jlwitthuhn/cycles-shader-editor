#pragma once

#include <memory>
#include <set>

#include "util_typedef.h"

namespace cse {

	class EditableNode;
	class FloatPos;
	class NodeSocket;

	enum class SelectMode {
		NORMAL,
		ADD,
		TOGGLE,
	};

	// This class is used to hold references to the objects currently selected by the user
	class Selection {
	public:
		void move_nodes(FloatPos delta);

		void modify_selection(SelectMode mode, std::weak_ptr<EditableNode> node);

		void clear();

		WeakNodeSet nodes;
		std::weak_ptr<NodeSocket> socket;
	};
}
