# Target executable
EXE := teste.exe
CC := g++

# Include and Library directories
INCS := 														  \
				-I.\lib\glfw\include 					\
				-I.\src -I.\lib\glad\include  \
				-I.\lib\spdlog\include        \
				-I.\lib\glm                   \
				-I.\lib\stb_image							\
				-I.\lib\imgui

LIBS := -L.\lib\glfw\ -lglfw3dll -lopengl32 -lws2_32 -lpthread

# Redistribuible libs
GLFW_DLL := .\lib\glfw\glfw3.dll

# Output directory and file
OUT_DIR := bin
OUT := $(OUT_DIR)\$(EXE)

# Compilation flags
C_FLAGS := -w -g -DSPDLOG_COMPILED_LIB -DSPDLOG_NO_EXCEPTIONS -std=c++20

# Source and object files
GLAD_OBJ := lib\glad\src\glad.o
STB_OBJ := lib\stb_image\stb_image.o
SRC := $(wildcard src/*.cpp) $(wildcard src/**/*.cpp)
SRC += $(wildcard lib/spdlog/src/*.cpp) # I could not get spdlog to link under MSVC nor GCC, so ill build it here.

# ImGui does not have a build system and thus, requires us to build it as part of the project.
SRC += $(wildcard lib/imgui/*.cpp) lib/imgui/backends/imgui_impl_glfw.cpp lib/imgui/backends/imgui_impl_opengl3.cpp

OBJ := $(SRC:.cpp=.o)

.SILENT:
.PHONY: all

# Default target
all: $(OUT) $(OUT_DIR)\glfw3.dll

# Copy out the library
$(OUT_DIR)\glfw3.dll:
	copy $(GLFW_DLL) $(OUT_DIR)\glfw3.dll

# Rule for building the executable
$(OUT): $(OBJ) $(GLAD_OBJ) $(STB_OBJ)
	$(CC) $(OBJ) $(GLAD_OBJ) $(STB_OBJ) -o $(OUT) $(INCS) $(LIBS)

# Rule for compiling .cpp files to .o (object files)
%.o: %.cpp
	@echo "Compiling file: $<"
	$(CC) -c $< -o $@ $(C_FLAGS) $(INCS)

# Rule for compiling glad.c separately
lib\glad\src\glad.o: lib\glad\src\glad.c
	$(CC) -c $< -o $@ $(C_FLAGS) $(INCS)

# Rule for compiling glad.c separately
# lib\glad\src\glad.o: lib\glad\src\glad.c
# 	$(CC) -c $< -o $@ $(C_FLAGS) $(INCS)

# Run the executable
run: all
	.\$(OUT)

clean:
	del /s /q *.o
