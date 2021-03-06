#pragma once

#include <memory>

#include "util_typedef.h"

namespace cse {

	class EditableNode;
	class Float2;
	class NodeSocket;

	enum class SelectMode {
		NORMAL,
		ADD,
		TOGGLE,
	};

	// This class is used to hold references to the objects currently selected by the user
	class Selection {
	public:
		void move_nodes(Float2 delta);

		void modify_selection(SelectMode mode, std::weak_ptr<EditableNode> node);

		void clear();

		WeakNodeSet nodes;
		std::weak_ptr<NodeSocket> socket;
	};
}
