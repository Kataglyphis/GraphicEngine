add_library(tinyobjloader INTERFACE)
# this helps us so no error messages for third party will be generated:
# https://stackoverflow.com/questions/49591804/clang-tidy-cmake-exclude-file-from-check
target_include_directories(tinyobjloader    SYSTEM
                                            INTERFACE  
                                            TOL)

add_library(stb INTERFACE)
target_include_directories(stb  SYSTEM 
                                INTERFACE 
                                STB)

add_library(imgui INTERFACE)
target_include_directories(imgui    SYSTEM 
                                    INTERFACE 
                                    IMGUI 
                                    IMGUI/backends)

add_library(glm INTERFACE)
target_include_directories(glm      SYSTEM 
                                    INTERFACE 
                                    GLM)

add_library(glad INTERFACE)
target_include_directories(glad     SYSTEM 
                                    INTERFACE 
                                    GLAD/include)

add_subdirectory(GLFW)
