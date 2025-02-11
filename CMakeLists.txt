cmake_minimum_required(VERSION 3.16)

set(EXEC_NAME "opengl")
project(${EXEC_NAME})

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)

# Set the output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include directories
include_directories(
        lib/glfw/include
        src
        lib/glad/include
        lib/spdlog/include
        lib/glm
        lib/stb_image
        lib/imgui
)

link_directories(
        lib/glfw
)

# Add the glad and stb_image object files
set(GLAD_SRC lib/glad/src/glad.c)
set(STB_IMAGE_SRC lib/stb_image/stb_image.cpp)

# Collect all source files
file(GLOB_RECURSE SRC_FILES
        src/*.cpp
        lib/spdlog/src/*.cpp
        lib/imgui/imgui.cpp
        lib/imgui/imgui_draw.cpp
        lib/imgui/imgui_tables.cpp
        lib/imgui/imgui_widgets.cpp
        lib/imgui/backends/imgui_impl_glfw.cpp
        lib/imgui/backends/imgui_impl_opengl3.cpp
)

# Add executable
add_executable(${EXEC_NAME} ${SRC_FILES} ${GLAD_SRC} ${STB_IMAGE_SRC})

# Link libraries
target_link_libraries(${EXEC_NAME}
        glfw3dll
        opengl32
        ws2_32
        pthread
)

# Compilation flags
target_compile_options(${EXEC_NAME} PRIVATE
        -w
        -g
        -DSPDLOG_COMPILED_LIB
        -DSPDLOG_NO_EXCEPTIONS
)

# Copy glfw3.dll to the output directory
add_custom_command(TARGET ${EXEC_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/lib/glfw/glfw3.dll
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/glfw3.dll
)

# Clean rule equivalent
set_directory_properties(PROPERTIES VS_STARTUP_PROJECT "${EXEC_NAME}")
