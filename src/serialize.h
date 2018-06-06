#pragma once

#include <list>
#include <string>
#include <vector>

namespace CyclesShaderEditor {

	struct OutputConnection;
	struct OutputNode;

	class EditorNode;
	class NodeConnection;

	void generate_output_lists(std::list<EditorNode*>& node_list, std::list<NodeConnection>& connection_list, std::vector<OutputNode>& out_node_list, std::vector<OutputConnection>& out_connection_list);

	std::string serialize_graph(std::vector<OutputNode>& nodes, std::vector<OutputConnection>& connections);
	void deserialize_graph(const std::string& graph, std::list<EditorNode*>& nodes, std::list<NodeConnection>& connections);

}