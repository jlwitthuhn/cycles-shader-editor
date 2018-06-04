#pragma once

#include <set>

#include "float_pos.h"

namespace CyclesShaderEditor {

	class EditorNode;
	class NodeSocket;

	// This class is used to hold references to the objects currently selected by the user
	class Selection {
	public:
		void move_nodes(FloatPos delta);

		std::set<EditorNode*> nodes;
		NodeSocket* socket = nullptr;
	};

}
