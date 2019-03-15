#include "graph_decoder.h"

#include <list>
#include <memory>

#include "node_base.h"
#include "serialize.h"

cse::CyclesNodeGraph::CyclesNodeGraph(const std::string& encoded_graph)
{
	// Temporary storage to fill during deserialization
	std::list<std::shared_ptr<EditorNode>> tmp_nodes;
	std::list<NodeConnection> tmp_connections;

	deserialize_graph(encoded_graph, tmp_nodes, tmp_connections);
	generate_output_lists(tmp_nodes, tmp_connections, nodes, connections);
}
