#pragma once

#include <memory>
#include <set>

namespace CyclesShaderEditor {

	class EditorNode;
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

		void modify_selection(SelectMode mode, EditorNode* node);

		void clear();

		std::set<EditorNode*> nodes;
		std::weak_ptr<NodeSocket> socket;
	};
}
