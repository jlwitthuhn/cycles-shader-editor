# Copy this to the project's top level to make it work
# Builds a static library of cycles-shader-graph_editor

cmake_minimum_required(VERSION 3.14)

include_directories("${NANOVG_INCLUDE_DIR}")
include_directories("${GLFW_INCLUDE_DIR}")
include_directories("${GLEW_INCLUDE_DIR}")

file(GLOB LibSources ./src/*.cpp)
add_library(neditor STATIC ${LibSources})
set_target_properties(neditor PROPERTIES PUBLIC_HEADER "./src/graph_decoder.h;./src/graph_editor.h;./src/output.h;./src/util_platform.h;./src/util_vector.h")

add_definitions(-DGLEW_STATIC)

install(TARGETS neditor
    LIBRARY DESTINATION ./lib
    PUBLIC_HEADER DESTINATION ./include/neditor
    )
