# Target executable
EXE := teste.exe
CC := g++

# Include and Library directories
INCS := -I.\lib\glfw\include -I.\src -I.\lib\glad\include
LIBS := -L.\lib\glfw\ -lglfw3dll -lopengl32

# Redistribuible libs
GLFW_DLL := .\lib\glfw\glfw3.dll

# Output directory and file
OUT_DIR := bin
OUT := $(OUT_DIR)\$(EXE)

# Compilation flags
C_FLAGS := -w -g -std=c++20

# Source and object files
GLAD_OBJ := lib\glad\src\glad.o
SRC := $(wildcard src/*.cpp) $(wildcard src/**/*.cpp)
OBJ := $(SRC:.cpp=.o)

# Default target
all: $(OUT) $(OUT_DIR)\glfw3.dll

# Copy out the library
$(OUT_DIR)\glfw3.dll:
	copy $(GLFW_DLL) $(OUT_DIR)\glfw3.dll

# Rule for building the executable
$(OUT): $(OBJ) $(GLAD_OBJ)
	$(CC) $(OBJ) $(GLAD_OBJ) -o $(OUT) $(INCS) $(LIBS)

# Rule for compiling .cpp files to .o (object files)
%.o: %.cpp
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
