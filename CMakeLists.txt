cmake_minimum_required(VERSION 3.14)

# set the project name
project(GraphicsEngine VERSION 1.0
                       DESCRIPTION "Epic Graphics"
                       LANGUAGES CXX)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# control where the static and shared libraries are built so that on windows
# we don't need to tinker with the path to run the executable
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

FILE(GLOB GUI_FILTER            "ExternalLib/IMGUI/*.h" 
                                "ExternalLib/IMGUI/backends/imgui_impl_glfw.h"
                                "ExternalLib/IMGUI/backends/imgui_impl_opengl3.h"
                                "ExternalLib/IMGUI/*.cpp"
                                "ExternalLib/IMGUI/backends/imgui_impl_glfw.cpp"
                                "ExternalLib/IMGUI/backends/imgui_impl_opengl3.cpp"
                                )

FILE(GLOB COMPUTE_FILTER                "Src/compute/*.h"                               "Src/compute/*.cpp")
FILE(GLOB RENDERER_FILTER               "Src/renderer/*.h"                              "Src/renderer/*.cpp")
FILE(GLOB CLOUD_FILTER                  "Src/scene/atmospheric_effects/clouds/*.h"      "Src/scene/atmospheric_effects/clouds/*.cpp")
FILE(GLOB LIGHT_FILTER                  "Src/scene/light/*.h"                           "Src/scene/light/*.cpp")
FILE(GLOB SHADOWS_FILTER                "Src/scene/shadows/*.h"                         "Src/scene/shadows/*.cpp")
FILE(GLOB SKY_BOX_FILTER                "Src/scene/sky_box/*.h"                         "Src/scene/sky_box/*.cpp")
FILE(GLOB TEXTURE_FILTER                "Src/scene/texture/*.h"                         "Src/scene/texture/*.cpp")
FILE(GLOB SCENE_FILTER                  "Src/scene/*.h"                                 "Src/scene/*.cpp")
FILE(GLOB MAIN_FILTER                   "Src/*.h"                                       "Src/*.cpp")

# add filters to the solution explorer for the shaders :))
file(GLOB SHADER_FILTER   
        ${PROJECT_BINARY_DIR}/../Resources/Shaders/*.vert
        ${PROJECT_BINARY_DIR}/../Resources/Shaders/*.frag
        ${PROJECT_BINARY_DIR}/../Resources/Shaders/*.glsl
        ${PROJECT_BINARY_DIR}/../Resources/Shaders/*.comp
	      ${PROJECT_BINARY_DIR}/../Resources/Shaders/*.geom
)

source_group("gui"                                      FILES ${GUI_FILTER})

source_group("shaders"                                  FILES ${SHADER_FILTER})

source_group("compute"                                  FILES ${COMPUTE_FILTER})
source_group("renderer"                                 FILES ${RENDERER_FILTER})

source_group("scene/atmospheric_effects/clouds"         FILES ${CLOUD_FILTER})
source_group("scene/lights"                             FILES ${LIGHT_FILTER})
source_group("scene/shadows"                            FILES ${SHADOWS_FILTER})
source_group("scene/sky_box"                            FILES ${SKY_BOX_FILTER})
source_group("scene/texture"                            FILES ${TEXTURE_FILTER})
source_group("scene"                                    FILES ${SCENE_FILTER})

source_group("main"                                     FILES ${MAIN_FILTER})

#set all your sources accordingly
file(GLOB_RECURSE SOURCES "Src/*.cpp"
                          "Src/*.c"
                          ${PROJECT_BINARY_DIR}/../ExternalLib/IMGUI/*.cpp
                          ${PROJECT_BINARY_DIR}/../ExternalLib/IMGUI/backends/imgui_impl_glfw.cpp
                          ${PROJECT_BINARY_DIR}/../ExternalLib/IMGUI/backends/imgui_impl_opengl3.cpp)

file(GLOB_RECURSE HEADERS "Src/*.h"
                          "Src/*.hpp"
                          ${PROJECT_BINARY_DIR}/../ExternalLib/IMGUI/*.h
                          ${PROJECT_BINARY_DIR}/../ExternalLib/IMGUI/backends/imgui_impl_glfw.h
                          ${PROJECT_BINARY_DIR}/../ExternalLib/IMGUI/backends/imgui_impl_opengl3.h)

# add the executable
add_executable(${PROJECT_NAME} 
                ${SHADER_FILTER} 
                ${GUI_FILTER}
                ${COMPUTE_FILTER}
                ${RENDERER_FILTER}
                ${ATMOSPHERE_EFFECTS_FILTER}
                ${LIGHT_FILTER}
                ${CLOUD_FILTER}
                ${SHADOWS_FILTER}
                ${SKY_BOX_FILTER}
                ${TEXTURE_FILTER}
                ${SCENE_FILTER}
                ${MAIN_FILTER})

if(UNIX)
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
else()
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
endif()

target_include_directories(${PROJECT_NAME}  PUBLIC "${PROJECT_BINARY_DIR}/../Src"
                                            PUBLIC "${PROJECT_BINARY_DIR}/../Src/compute"
                                            PUBLIC "${PROJECT_BINARY_DIR}/../Src/renderer"
                                            PUBLIC "${PROJECT_BINARY_DIR}/../Src/scene/atmospheric_effects/clouds"
                                            PUBLIC "${PROJECT_BINARY_DIR}/../Src/scene/light"
                                            PUBLIC "${PROJECT_BINARY_DIR}/../Src/scene/shadows"
                                            PUBLIC "${PROJECT_BINARY_DIR}/../Src/scene/sky_box"
                                            PUBLIC "${PROJECT_BINARY_DIR}/../Src/scene/texture"
                                            PUBLIC "${PROJECT_BINARY_DIR}/../Src/scene"
                                            PUBLIC "${PROJECT_BINARY_DIR}/../ExternalLib/GLM"
                                            PUBLIC "${PROJECT_BINARY_DIR}/../ExternalLib/IMGUI"
                                            PUBLIC "${PROJECT_BINARY_DIR}/../ExternalLib/STB"
                                            PUBLIC "${PROJECT_BINARY_DIR}/../ExternalLib/TOL"
                                            PUBLIC "${PROJECT_BINARY_DIR}/../ExternalLib/IMGUI/backends"
                                            PUBLIC "${PROJECT_BINARY_DIR}/../ExternalLib/GLEW/glew-2.2.0/include/GL"
                                            PUBLIC "${PROJECT_BINARY_DIR}/../ExternalLib/GLEW/glew-2.2.0/include"
                                            PUBLIC "${PROJECT_BINARY_DIR}/../ExternalLib/GLFW/glfw-3.3.4/include/GLFW"
                                            ${OPENGL_INCLUDE_DIRS})

                                            # we depend on OpenGL
find_package( OpenGL REQUIRED COMPONENTS OpenGL)
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})

#surpress warnings
set(OpenGL_GL_PREFERENCE GLVND)

# set all glfw dependencies
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(ExternalLib/GLFW/glfw-3.3.4)

target_link_libraries(${PROJECT_NAME} glfw)

# set all glew dependencies
add_subdirectory(ExternalLib/GLEW/glew-2.2.0/build/cmake)
target_link_libraries(${PROJECT_NAME} glew)

# add tests 
include(CTest)

add_test(test1 ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME})

set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "${GraphicsEngine_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${GraphicsEngine_VERSION_MINOR}")

set_target_properties( ${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY 
                       ${PROJECT_BINARY_DIR})

set_target_properties( ${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY
                       ${PROJECT_BINARY_DIR})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} ${CMAKE_DL_LIBS}
                                      Threads::Threads)


if(UNIX)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g")
else()
endif()

# first we can indicate the documentation build as an option and set it to ON by default
option(BUILD_DOC "Build documentation" ON)

# check if Doxygen is installed
find_package(Doxygen)
if (DOXYGEN_FOUND)
    # set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_BINARY_DIR}/../Documents/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")

    # note the option ALL which allows to build the docs together with the application
    add_custom_target( doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )
else (DOXYGEN_FOUND)
  message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  hello_test
  Test/hello_test.cc
)
target_link_libraries(
  hello_test
  gtest_main
)

include(GoogleTest)
gtest_discover_tests(hello_test)

include(CPack)
