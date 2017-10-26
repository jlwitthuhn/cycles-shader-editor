# Cycles Shader Editor

Cycles Shader Editor is a cross-platform C++ library that provides a graphical editor for creating [Cycles](https://www.cycles-renderer.org/) shader graphs with a simple drag and drop interface. It is intended to be used as part of a plugin that integrates Cycles with a DCC app and was created as part of [my own plugin for 3ds Max](https://cyclesformax.net/).

## Running the Example Program

Included in this repository is a very simple program that creates a node graph editor window and, when the user saves, writes the serialized node graph to stdout.

### Prerequisites

This should build on any plaform that GLFW builds on, but I have only tested it with Visual Studio 2015 for Windows 7+, GCC 7.2.1 on Fedora 26, and Clang 4.0.1 on Fedora 26.

Libraries you will need are:
- GLFW 3.x
- NanoVG
  - A compatible version is bundled in this repository, but any moderately recent version should work.
- Recent-ish GLEW
  - This is not needed on Windows.

OpenGL 2.0 support is required to run the editor.

This project does not depend on any Cycles code or headers.

### Building

On unixy systems, you can simply run 'make' to build the example program. It will produce a binary named 'shader_editor' in the top-level directory. It will also produce a 'lib/libshadereditor.a' static library.

On Windows I haven't provided a visual studio project, but it should build fine if you put all the source files together in a project with default settings. You will also need to define the preprocessor symbol NANOVG_GL2_IMPLEMENTATION so NanoVG knows what OpenGL implementation to use.

### Using the Editor

* To create nodes, drag and drop a button from the node list onto the grid.
* To create connections, drag from the output of one node to the input of another.
* To pan the view, you can use the arrow keys on your keyboard or middle-click and drag anywhere on the grid.
* To write the serialized version of your graph to stdout, click the save button or press ctrl+s.

## Calling the Shader Editor from Your Code

You will need some headers to be able to use this library's functionality. With the makefile included here, these will automatically get copied to the 'include' top-level directory. For those not using the makefile, these headers are:

* graph_decoder.h
* graph_editor.h
* output.h
* util_platform.h

All types defined in these headers are a part of the CyclesShaderEditor namespace.

### Creating a Window

The included example program is close to the smallest program possible to make with the library. It is reproduced below.

```C++
#include <iostream>
#include <graph_editor.h>

int main()
{
	CyclesShaderEditor::GraphEditor node_window;
	node_window.create_window();
	while (node_window.run_window_loop_iteration()) {
		if (node_window.output_updated) {
			std::cout << "Graph saved:\n" << node_window.serialized_output << std::endl;
			node_window.output_updated = false;
		}
	}
	return 0;
}
```

You will use largely the same basic flow when using this library. The key points here are:

* CyclesShaderEditor::GraphEditor is the main class used to represent a Node Graph Editor window.
* Calling GraphEditor::create_window() will cause the window to appear.
* Once a window exists, loop calling GraphEditor::run_window_loop_iteration() until it returns false.
  * This method is responsible for handling user input and drawing
  * It will return false once the window has been closed
* If GraphEditor::output_updated is true, serialized_output will contain a serialized node graph string.
  * When this string is copied from the window object, output_updated should be set to false. It will be set to true when the user saves again.

### Decoding the Graph String

Now you can create a window and get a serialized graph from it, but you don't have any way to produce a Cycles ccl::ShaderGraph from that serialized form.

To help with this, you will need to use the CyclesShaderEditor::CyclesNodeGraph class defined in "graph_decoder.h". This class has a single constructor that takes a serialized graph string as an argument. Once the object construction is complete, the 'nodes' and 'connections' members will be populated with relevant information.

### Construction a ccl::ShaderGraph

The file [extra/shader_graph_converter.cpp](extra/shader_graph_converter.cpp) contains some functions that can be used to create a ccl::ShaderGraph from a serialized graph string. These are not included in the main project to avoid requiring Cycles as a dependency for building the editor.

## The Serialized Graph Format

The format for serialized graph strings is very simple. It is a series of pipe-delimited strings. The basic format of the string is:
```
cycles_shader|1|section_nodes|[NODES]|section_connections|[CONNECTIONS]|
^             ^ ^             ^       ^                   ^
1             2 3             4       5                   6
```
1. special string to identify shader graph files
2. serialization format version
3. nodes follow this marker
4. list of nodes
5. connections follow this marker
6. list of connections

The format of each node is:
```
TYPE|NAME|X_POS|Y_POS|INPUT_NAME|INPUT_VALUE|...|node_end
```

* TYPE is the type of the node, such as diffuse_bsdf
* NAME is a unique identifier for the node
* X_POS and Y_POS are for the position of the node inside the graph editor
* INPUT_NAME and INPUT_VALUE can be repeated any number of times to cover all of a node's inputs

The format of each connection is:
```
SRC_NODE|SRC_SLOT|DST_NODE|DST_SLOT
```
* SRC_NODE and DST_NODE are node names, as described above
* SRC_SLOT and DST_SLOT are socket names

Bringing this all together, a simple diffuse shader will have a serialized graph that resembles this:
```
cycles_shader|1|section_nodes|diffuse_bsdf|node1|0|0|roughness|0|color|0,1,1|node_end|out_material|output|200|0|node_end|section_connections|node1|BSDF|output|Surface|
```

## License

This project is available under the zlib license. The full text of the license is available in [LICENSE.txt](LICENSE.txt)
