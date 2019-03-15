#pragma once

#include <list>
#include <memory>
#include <string>
#include <vector>

namespace cse {

	struct OutputConnection;
	struct OutputNode;

	class EditableNode;
	class NodeConnection;

	void generate_output_lists(
		const std::list<std::shared_ptr<EditableNode>>& node_list,
		const std::list<NodeConnection>& connection_list,
		std::vector<OutputNode>& out_node_list,
		std::vector<OutputConnection>& out_connection_list
	);

	std::string serialize_graph(std::vector<OutputNode>& nodes, std::vector<OutputConnection>& connections);
	void deserialize_graph(
		const std::string& graph,
		std::list<std::shared_ptr<EditableNode>>& nodes,
		std::list<NodeConnection>& connections
	);

}