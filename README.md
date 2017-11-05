# Cycles Shader Editor

Cycles Shader Editor is a cross-platform C++ library that provides a graphical editor for creating [Cycles](https://www.cycles-renderer.org/) shader graphs with a simple drag and drop interface. It is intended to be used as part of a plugin that integrates Cycles with a DCC app and was created as part of [my own plugin for 3ds Max](https://cyclesformax.net/).

![Screenshot](/extra/screenshot.png)

## Running the Example Program

Included in this repository is a simple program that creates a node graph editor window and, when the user saves, writes the serialized node graph to stdout.

### Prerequisites

This should build on any plaform that GLFW builds on, but I have only tested it with Visual Studio 2015, GCC 7.2.1 on Fedora 26, Clang 4.0.1 on Fedora 26, and Apple Clang 9.0.0 on macOS 10.13

Libraries you will need are:
- GLFW 3.x
- GLEW
- NanoVG
  - A compatible version is bundled in this repository, but any moderately recent version should work.

OpenGL 2.0 support is required to run the editor.

This project does not depend on any Cycles code or headers.

### Building

On unixy systems, you can simply run `make` to build the example program. It will produce a binary named `shader_editor` in the top-level directory. It will also produce a `lib/libshadereditor.a` static library.

On Windows I haven't provided a visual studio project, but it should build fine if you put all the source files together in a C++ project with default settings. You will also need to define the preprocessor symbol `NANOVG_GL2_IMPLEMENTATION` so NanoVG knows what OpenGL implementation to use.

### Using the Editor

* To create a node, drag and drop from a button in the 'Nodes' window onto the grid.
* To create a connection, click and drag from the output circle of one node to the input of another.
* To change a parameter, click the parameter's name on the node.
* To pan the view, use the arrow keys on your keyboard or middle-click and drag anywhere on the grid.
* To write the serialized version of your graph to stdout, click the save button or press ctrl+s.

## Calling the Shader Editor from Your Code

You will need just a few headers to be able to use this library's functionality. With the makefile included here, these headers will automatically get copied to the `include` top-level directory during the build. For those not using the makefile, these headers are:

* graph_decoder.h
* graph_editor.h
* output.h
* util_platform.h

All types defined in these headers are a part of the CyclesShaderEditor namespace. `graph_decoder.h` and `graph_editor.h` are the only two you will need to #include directly, the others define types used by graph_decoder and graph_editor. 

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
  * This method is responsible for handling user input and drawing.
  * It will return false once the window has been closed.
* If GraphEditor::output_updated is true, serialized_output will contain a serialized node graph string.
  * When this string is read from the window object, output_updated should be set to false. It will be set to true when the user saves again.

### Decoding the Graph String

Now you can create a window and get a serialized graph from it, but that string is not very useful on its own.

To help with this, you can use the CyclesShaderEditor::CyclesNodeGraph class defined in `graph_decoder.h`. This class has a single constructor that takes a serialized graph string as an argument. Once the object construction is complete, the 'nodes' and 'connections' members will be populated with relevant information.

### Constructing a ccl::ShaderGraph

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
