CC ?= gcc
CXX ?= g++
AR ?= ar

BINARY_NAME = shader_editor
LIB_NAME = libshadereditor.a

SRC_DIR = ./src
INC_DIR = ./include
OBJ_DIR = ./obj
LIB_DIR = ./lib

LIB_PATH := $(LIB_DIR)/$(LIB_NAME)

CPP_PATHS := $(shell find $(SRC_DIR) -name *.cpp)
CPP_FILES := $(notdir ${CPP_PATHS})
OBJ_PATHS := $(CPP_FILES:%=$(OBJ_DIR)/%.o)
GCC_MAKEFILES := $(OBJ_PATHS:.o=.d)

DEFINES := NANOVG_GL2_IMPLEMENTATION
DEFINES_FLAGS := $(addprefix -D,$(DEFINES))

CPPFLAGS_NVG := -MMD -MP $(DEFINES_FLAGS)
CPPFLAGS := -MMD -MP $(DEFINES_FLAGS) -Inanovg/src/
CXXFLAGS = -Wall -std=c++11
LDFLAGS = -lstdc++ -lm -lGL -lGLEW -lglfw

MKDIR_P = mkdir -p

PUBLIC_INCLUDES = graph_decoder.h graph_editor.h output.h util_platform.h
PUBLIC_INCLUDE_DST := $(addprefix $(INC_DIR)/,$(PUBLIC_INCLUDES))

$(BINARY_NAME): $(LIB_PATH) $(PUBLIC_INCLUDE_DST) 
	$(MKDIR_P) $(dir $@)
	$(CXX) ./example/main.cpp $(LIB_DIR)/$(LIB_NAME) $(CXXFLAGS) $(LDFLAGS) -I$(INC_DIR) -o $@

$(OBJ_DIR)/%.cpp.o: $(SRC_DIR)/%.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/nanovg.o: nanovg/src/nanovg.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS_NVG) -c $< -o $@

$(INC_DIR)/%.h: $(SRC_DIR)/%.h
	$(MKDIR_P) $(dir $@)
	cp $< $@

$(LIB_PATH): $(OBJ_PATHS) $(OBJ_DIR)/nanovg.o
	$(MKDIR_P) $(dir $@)
	$(AR) rcs $(LIB_DIR)/$(LIB_NAME) $^

-include $(GCC_MAKEFILES)

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(INC_DIR)
	rm -rf $(LIB_DIR)

