add_library(tinyobjloader INTERFACE)
target_include_directories(tinyobjloader INTERFACE TOL)

add_library(stb INTERFACE)
target_include_directories(stb INTERFACE STB)

add_library(imgui INTERFACE)
target_include_directories(imgui INTERFACE IMGUI IMGUI/backends)

add_library(glm INTERFACE)
target_include_directories(glm INTERFACE GLM)
