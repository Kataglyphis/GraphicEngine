cmake_minimum_required(VERSION 3.14)

# set the project name
project(GraphicsEngine VERSION 1.9
                       DESCRIPTION "Epic Graphics"
                       LANGUAGES CXX C)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# include google test to project
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# for removing dirty file directories in the cpp-code
add_compile_definitions(CMAKELISTS_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

# control where the static and shared libraries are built so that on windows
# we don't need to tinker with the path to run the executable
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

# add all external libs as interface
add_subdirectory(ExternalLib)

# set all glfw dependencies
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(ExternalLib/GLFW)

# we depend on OpenGL
find_package( OpenGL REQUIRED COMPONENTS OpenGL)
#surpress warnings
set(OpenGL_GL_PREFERENCE GLVND)

# for faster building
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

FILE(GLOB GUI_FILTER            "ExternalLib/IMGUI/*.h" 
                                "ExternalLib/IMGUI/backends/imgui_impl_glfw.h"
                                "ExternalLib/IMGUI/backends/imgui_impl_opengl3.h"
                                "ExternalLib/IMGUI/*.cpp"
                                "ExternalLib/IMGUI/backends/imgui_impl_glfw.cpp"
                                "ExternalLib/IMGUI/backends/imgui_impl_opengl3.cpp"
                                "Src/gui/*.h"                               "Src/gui/*.cpp")

FILE(GLOB COMPUTE_FILTER                "Src/compute/*.h"                               "Src/compute/*.cpp")

FILE(GLOB CAMERA_FILTER                 "Src/camera/*.h"                                "Src/camera/*.cpp")

FILE(GLOB RENDERER_FILTER               "Src/renderer/*.h"                              "Src/renderer/*.cpp")
FILE(GLOB DEFERRED_FILTER               "Src/renderer/deferred/*.h"                     "Src/renderer/deferred/*.cpp")
FILE(GLOB LOAD_SCREEN_FILTER            "Src/renderer/loading_screen/*.h"               "Src/renderer/loading_screen/*.cpp")

FILE(GLOB CLOUD_FILTER                  "Src/scene/atmospheric_effects/clouds/*.h"      "Src/scene/atmospheric_effects/clouds/*.cpp")

FILE(GLOB LIGHT_FILTER                  "Src/scene/light/*.h"                           "Src/scene/light/*.cpp")
FILE(GLOB D_LIGHT_FILTER                "Src/scene/light/directional_light/*.h"         "Src/scene/light/directional_light/*.cpp")
FILE(GLOB P_LIGHT_FILTER                "Src/scene/light/point_light/*.h"               "Src/scene/light/point_light/*.cpp")

FILE(GLOB SHADOWS_FILTER                "Src/scene/shadows/*.h"                         "Src/scene/shadows/*.cpp")
FILE(GLOB SKY_BOX_FILTER                "Src/scene/sky_box/*.h"                         "Src/scene/sky_box/*.cpp")
FILE(GLOB TEXTURE_FILTER                "Src/scene/texture/*.h"                         "Src/scene/texture/*.cpp")
FILE(GLOB SCENE_FILTER                  "Src/scene/*.h"                                 "Src/scene/*.cpp")

FILE(GLOB WINDOW_FILTER                 "Src/window/*.h"                                "Src/window/*.cpp")
FILE(GLOB DEBUG_FILTER                  "Src/debug/*.h"                                 "Src/debug/*.cpp")
FILE(GLOB UTIL_FILTER                   "Src/util/*.h"                                  "Src/util/*.cpp")
FILE(GLOB MAIN_FILTER                   "Src/*.h"                                       "Src/*.cpp")

FILE(GLOB GLAD_FILTER                   "ExternalLib/GLAD/include/glad/*.h"             "ExternalLib/GLAD/src/*.c"
                                        "ExternalLib/GLAD/include/KHR/*.h")

# add filters to the solution explorer for the shaders :))
file(GLOB SHADER_FILTER   
        ${PROJECT_BINARY_DIR}/../Resources/Shaders/*.vert
        ${PROJECT_BINARY_DIR}/../Resources/Shaders/*.frag
        ${PROJECT_BINARY_DIR}/../Resources/Shaders/*.glsl
        ${PROJECT_BINARY_DIR}/../Resources/Shaders/*.comp
	${PROJECT_BINARY_DIR}/../Resources/Shaders/*.geom
)

file(GLOB PBR_SHADER_FILTER   
        ${PROJECT_BINARY_DIR}/../Resources/Shaders/pbr/*.vert
        ${PROJECT_BINARY_DIR}/../Resources/Shaders/pbr/*.frag
        ${PROJECT_BINARY_DIR}/../Resources/Shaders/pbr/*.glsl
        ${PROJECT_BINARY_DIR}/../Resources/Shaders/pbr/*.comp
	${PROJECT_BINARY_DIR}/../Resources/Shaders/pbr/*.geom
)

file(GLOB BRDF_SHADER_FILTER   
        ${PROJECT_BINARY_DIR}/../Resources/Shaders/pbr/brdf/*.vert
        ${PROJECT_BINARY_DIR}/../Resources/Shaders/pbr/brdf/*.frag
        ${PROJECT_BINARY_DIR}/../Resources/Shaders/pbr/brdf/*.glsl
        ${PROJECT_BINARY_DIR}/../Resources/Shaders/pbr/brdf/*.comp
	${PROJECT_BINARY_DIR}/../Resources/Shaders/pbr/brdf/*.geom
)

file(GLOB COMMON_SHADER_FILTER   
        ${PROJECT_BINARY_DIR}/../Resources/Shaders/common/*.vert
        ${PROJECT_BINARY_DIR}/../Resources/Shaders/common/*.frag
        ${PROJECT_BINARY_DIR}/../Resources/Shaders/common/*.glsl
        ${PROJECT_BINARY_DIR}/../Resources/Shaders/common/*.comp
	${PROJECT_BINARY_DIR}/../Resources/Shaders/common/*.geom
)

file(GLOB CLOUDS_SHADER_FILTER   
        ${PROJECT_BINARY_DIR}/../Resources/Shaders/clouds/*.vert
        ${PROJECT_BINARY_DIR}/../Resources/Shaders/clouds/*.frag
        ${PROJECT_BINARY_DIR}/../Resources/Shaders/clouds/*.glsl
        ${PROJECT_BINARY_DIR}/../Resources/Shaders/clouds/*.comp
	${PROJECT_BINARY_DIR}/../Resources/Shaders/clouds/*.geom
)

file(GLOB LOADING_SHADER_FILTER   
        ${PROJECT_BINARY_DIR}/../Resources/Shaders/loading_screen/*.vert
        ${PROJECT_BINARY_DIR}/../Resources/Shaders/loading_screen/*.frag
        ${PROJECT_BINARY_DIR}/../Resources/Shaders/loading_screen/*.glsl
        ${PROJECT_BINARY_DIR}/../Resources/Shaders/loading_screen/*.comp
	${PROJECT_BINARY_DIR}/../Resources/Shaders/loading_screen/*.geom
)

file(GLOB RASTERIZER_SHADER_FILTER   
        ${PROJECT_BINARY_DIR}/../Resources/Shaders/rasterizer/*.vert
        ${PROJECT_BINARY_DIR}/../Resources/Shaders/rasterizer/*.frag
        ${PROJECT_BINARY_DIR}/../Resources/Shaders/rasterizer/*.glsl
        ${PROJECT_BINARY_DIR}/../Resources/Shaders/rasterizer/*.comp
	${PROJECT_BINARY_DIR}/../Resources/Shaders/rasterizer/*.geom
)

file(GLOB RASTERIZER_SHADOWS_SHADER_FILTER   
        ${PROJECT_BINARY_DIR}/../Resources/Shaders/rasterizer/shadows/*.vert
        ${PROJECT_BINARY_DIR}/../Resources/Shaders/rasterizer/shadows/*.frag
        ${PROJECT_BINARY_DIR}/../Resources/Shaders/rasterizer/shadows/*.glsl
        ${PROJECT_BINARY_DIR}/../Resources/Shaders/rasterizer/shadows/*.comp
	${PROJECT_BINARY_DIR}/../Resources/Shaders/rasterizer/shadows/*.geom
)

file(GLOB SKYBOX_SHADER_FILTER   
        ${PROJECT_BINARY_DIR}/../Resources/Shaders/skybox/*.vert
        ${PROJECT_BINARY_DIR}/../Resources/Shaders/skybox/*.frag
        ${PROJECT_BINARY_DIR}/../Resources/Shaders/skybox/*.glsl
        ${PROJECT_BINARY_DIR}/../Resources/Shaders/skybox/*.comp
	${PROJECT_BINARY_DIR}/../Resources/Shaders/skybox/*.geom
)

source_group("gui"                                      FILES ${GUI_FILTER})

source_group("shaders"                                  FILES ${SHADER_FILTER})
source_group("shaders/pbr"                              FILES ${PBR_SHADER_FILTER})
source_group("shaders/pbr/brdf"                         FILES ${BRDF_SHADER_FILTER})
source_group("shaders/common"                           FILES ${COMMON_SHADER_FILTER})
source_group("shaders/clouds"                           FILES ${CLOUDS_SHADER_FILTER})
source_group("shaders/loading_screen"                   FILES ${LOADING_SHADER_FILTER})
source_group("shaders/rasterizer"                       FILES ${RASTERIZER_SHADER_FILTER})
source_group("shaders/rasterizer/shadows"               FILES ${RASTERIZER_SHADOWS_SHADER_FILTER})
source_group("shaders/skybox"                           FILES ${SKYBOX_SHADER_FILTER})

source_group("compute"                                  FILES ${COMPUTE_FILTER})

source_group("camera"                                   FILES ${CAMERA_FILTER})

source_group("renderer"                                 FILES ${RENDERER_FILTER})
source_group("renderer/deferred"                        FILES ${DEFERRED_FILTER})
source_group("renderer/loading_screen"                  FILES ${LOAD_SCREEN_FILTER})

source_group("window"                                   FILES ${WINDOW_FILTER})
source_group("debug"                                    FILES ${DEBUG_FILTER})
source_group("util"                                     FILES ${UTIL_FILTER})

source_group("glad"                                     FILES ${GLAD_FILTER})

source_group("scene/atmospheric_effects/clouds"         FILES ${CLOUD_FILTER})

source_group("scene/lights"                             FILES ${LIGHT_FILTER})
source_group("scene/lights/directional"                 FILES ${D_LIGHT_FILTER})
source_group("scene/lights/point"                       FILES ${P_LIGHT_FILTER})

source_group("scene/shadows"                            FILES ${SHADOWS_FILTER})
source_group("scene/sky_box"                            FILES ${SKY_BOX_FILTER})
source_group("scene/texture"                            FILES ${TEXTURE_FILTER})
source_group("scene"                                    FILES ${SCENE_FILTER})

source_group("app"                                      FILES ${MAIN_FILTER})

#set all your sources accordingly
file(GLOB_RECURSE SOURCES "Src/*.cpp"
                          "Src/*.c"
                          ${PROJECT_BINARY_DIR}/../ExternalLib/GLAD/*.c
                          ${PROJECT_BINARY_DIR}/../ExternalLib/IMGUI/*.cpp
                          ${PROJECT_BINARY_DIR}/../ExternalLib/IMGUI/backends/imgui_impl_glfw.cpp
                          ${PROJECT_BINARY_DIR}/../ExternalLib/IMGUI/backends/imgui_impl_opengl3.cpp)

file(GLOB_RECURSE HEADERS "Src/*.h"
                          "Src/*.hpp"
                          ${PROJECT_BINARY_DIR}/../ExternalLib/GLAD/*.h
                          ${PROJECT_BINARY_DIR}/../ExternalLib/IMGUI/*.h
                          ${PROJECT_BINARY_DIR}/../ExternalLib/IMGUI/backends/imgui_impl_glfw.h
                          ${PROJECT_BINARY_DIR}/../ExternalLib/IMGUI/backends/imgui_impl_opengl3.h)

# add the executable
add_executable( ${PROJECT_NAME} 
                ${SHADER_FILTER}
                ${PBR_SHADER_FILTER}
                ${BRDF_SHADER_FILTER} 
                ${CAMERA_FILTER} 
                ${COMMON_SHADER_FILTER}  
                ${CLOUDS_SHADER_FILTER}
                ${LOADING_SHADER_FILTER}
                ${RASTERIZER_SHADER_FILTER}
                ${RASTERIZER_SHADOWS_SHADER_FILTER}
                ${SKYBOX_SHADER_FILTER}
                ${GUI_FILTER}
                ${GLAD_FILTER}
                ${WINDOW_FILTER}
                ${DEBUG_FILTER}
                ${UTIL_FILTER}
                ${COMPUTE_FILTER}
                ${RENDERER_FILTER}
                ${DEFERRED_FILTER}
                ${LOAD_SCREEN_FILTER}
                ${ATMOSPHERE_EFFECTS_FILTER}
                ${LIGHT_FILTER}
                ${D_LIGHT_FILTER}
                ${P_LIGHT_FILTER}
                ${CLOUD_FILTER}
                ${SHADOWS_FILTER}
                ${SKY_BOX_FILTER}
                ${TEXTURE_FILTER}
                ${SCENE_FILTER}
                ${MAIN_FILTER})

target_include_directories(${PROJECT_NAME}      PUBLIC "./Src"
                                                PUBLIC "./Src/camera"
                                                PUBLIC "./Src/compute"
                                                PUBLIC "./Src/renderer"
                                                PUBLIC "./Src/renderer/loading_screen"
                                                PUBLIC "./Src/renderer/deferred"
                                                PUBLIC "./Src/debug"
                                                PUBLIC "./Src/window"
                                                PUBLIC "./Src/scene"
                                                PUBLIC "./Src/scene/atmospheric_effects/clouds"
                                                PUBLIC "./Src/scene/light"
                                                PUBLIC "./Src/scene/light/directional_light"
                                                PUBLIC "./Src/scene/light/point_light"
                                                PUBLIC "./Src/scene/shadows"
                                                PUBLIC "./Src/scene/sky_box"
                                                PUBLIC "./Src/scene/texture"
                                                PUBLIC "./Src/gui"
                                                PUBLIC "./Src/util"  
                                                PUBLIC "./ExternalLib/GLAD/include"
                                                ${OPENGL_INCLUDE_DIRS})


target_link_libraries(${PROJECT_NAME}   PUBLIC  ${CMAKE_DL_LIBS} # for imgui
                                                Threads::Threads
                                                ${OPENGL_LIBRARIES}
                                                glfw
                                                imgui
                                                stb 
                                                glm 
                                                tinyobjloader)


set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "${GraphicsEngine_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${GraphicsEngine_VERSION_MINOR}")

# we really need this; glew already build its libraries in bin/ folder
# otherwise program will not find our library!
set_target_properties( ${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                                    ${PROJECT_BINARY_DIR})

set_target_properties( ${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY
                       ${PROJECT_BINARY_DIR})

# let us print all errors when on UNIX
if(UNIX)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g")
else()
endif()

# all testing related stuff
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

# first we can indicate the documentation build as an option and set it to ON by default
# option(BUILD_DOC "Build documentation" ON)

# check if Doxygen is installed
#find_package(Doxygen)
#if (DOXYGEN_FOUND)
    # set input and output files
 #   set(DOXYGEN_IN ${CMAKE_CURRENT_BINARY_DIR}/../Documents/Doxyfile.in)
  #  set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # request to configure the file
   # configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
   # message("Doxygen build started")

    # note the option ALL which allows to build the docs together with the application
   # add_custom_target( doc_doxygen ALL
    #    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
     #   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      #  COMMENT "Generating API documentation with Doxygen"
       # VERBATIM )
#else (DOXYGEN_FOUND)
 # message("Doxygen need to be installed to generate the doxygen documentation")
#endif (DOXYGEN_FOUND)
