#pragma once

#include <string>
#include <vector>

#include "output.h"

namespace cse {

	// Description of a Cycles shader graph
	class CyclesNodeGraph {
	public:
		CyclesNodeGraph(const std::string& encoded_graph);

		std::vector<OutputNode> nodes;
		std::vector<OutputConnection> connections;
	};

}
