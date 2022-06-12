cmake_minimum_required(VERSION 3.18)

# set the project name
project(GraphicsEngine VERSION 1.9
                       DESCRIPTION "Epic Graphics"
                       LANGUAGES CXX C)

# specify the C/C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)

# link this library for using warnings specified in cmake/CmakeWarnings.cmake
add_library(project_warnings INTERFACE)
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# add sanitizers if GCC/CLANG
add_library(project_sanitizers INTERFACE)
include(cmake/Sanitizers.cmake)
enable_sanitizers(project_sanitizers)

include(cmake/Doxygen.cmake)
enable_doxygen()

include(cmake/StaticAnalyzers.cmake)

include(FetchContent)
# include google test to project
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/e2239ee6043f73722e7aa812a459f54a28552929.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

#include gsl
# In this example we are picking a specific tag.
# You can also pick a specific commit, if you need to.
FetchContent_Declare(GSL
    GIT_REPOSITORY "https://github.com/microsoft/GSL"
    GIT_TAG "v4.0.0"
)
FetchContent_MakeAvailable(GSL)

# for removing dirty file directories in the cpp-code
# add_compile_definitions(CMAKELISTS_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

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
# configure OpenGL version
set(OPENGL_VERSION_MAJOR 4)
set(OPENGL_VERSION_MINOR 6)
#surpress warnings
set(OpenGL_GL_PREFERENCE GLVND)

# for faster building
find_package(Threads REQUIRED)

# https://stackoverflow.com/questions/1027247/is-it-better-to-specify-source-files-with-glob-or-each-file-individually-in-cmak
# https://stackoverflow.com/questions/45090926/cmake-generate-list-of-source-files-without-glob
# lets remove the FILE GLOB ... it is bad (as stated in the docs ... https://cmake.org/cmake/help/latest/command/file.html)

FILE(GLOB GUI_FILTER                    "Src/gui/*.h"                                           "Src/gui/*.cpp")
FILE(GLOB IMGUI_FILTER                  "ExternalLib/IMGUI/*.h"                                 "ExternalLib/IMGUI/*.cpp"
                                        "ExternalLib/IMGUI/backends/imgui_impl_glfw.h"          "ExternalLib/IMGUI/backends/imgui_impl_glfw.cpp"
                                        "ExternalLib/IMGUI/backends/imgui_impl_opengl3.h"       "ExternalLib/IMGUI/backends/imgui_impl_opengl3.cpp")

FILE(GLOB COMPUTE_FILTER                "Src/compute/*.h"                                       "Src/compute/*.cpp")

FILE(GLOB CAMERA_FILTER                 "Src/camera/*.h"                                        "Src/camera/*.cpp")

FILE(GLOB RENDERER_FILTER               "Src/renderer/*.h"                                      "Src/renderer/*.cpp")
FILE(GLOB DEFERRED_FILTER               "Src/renderer/deferred/*.h"                             "Src/renderer/deferred/*.cpp")
FILE(GLOB LOAD_SCREEN_FILTER            "Src/renderer/loading_screen/*.h"                       "Src/renderer/loading_screen/*.cpp")

FILE(GLOB CLOUD_FILTER                  "Src/scene/atmospheric_effects/clouds/*.h"              "Src/scene/atmospheric_effects/clouds/*.cpp")

FILE(GLOB LIGHT_FILTER                  "Src/scene/light/*.h"                                   "Src/scene/light/*.cpp")
FILE(GLOB D_LIGHT_FILTER                "Src/scene/light/directional_light/*.h"                 "Src/scene/light/directional_light/*.cpp")
FILE(GLOB P_LIGHT_FILTER                "Src/scene/light/point_light/*.h"                       "Src/scene/light/point_light/*.cpp")

FILE(GLOB SHADOWS_FILTER                "Src/scene/shadows/*.h"                                 "Src/scene/shadows/*.cpp")
FILE(GLOB SKY_BOX_FILTER                "Src/scene/sky_box/*.h"                                 "Src/scene/sky_box/*.cpp")
FILE(GLOB TEXTURE_FILTER                "Src/scene/texture/*.h"                                 "Src/scene/texture/*.cpp")
FILE(GLOB SCENE_FILTER                  "Src/scene/*.h"                                         "Src/scene/*.cpp")

FILE(GLOB WINDOW_FILTER                 "Src/window/*.h"                                        "Src/window/*.cpp")
FILE(GLOB DEBUG_FILTER                  "Src/debug/*.h"                                         "Src/debug/*.cpp")
FILE(GLOB UTIL_FILTER                   "Src/util/*.h"                                          "Src/util/*.cpp")

FILE(GLOB APP_FILTER                    "Src/app/*.h"                                           "Src/app/*.cpp")

FILE(GLOB COMMON_FILTER                 "Src/*.h"                                               "Src/*.cpp")

FILE(GLOB GLAD_FILTER                   "ExternalLib/GLAD/include/glad/*.h"                     "ExternalLib/GLAD/src/*.c"
                                        "ExternalLib/GLAD/include/KHR/*.h")

FILE(GLOB SHADER_HOST_DEVICE_FILTER     "Resources/Shaders/hostDevice/*.h"                     "Resources/Shaders/hostDevice/*.cpp"
                                        "Resources/Shaders/hostDevice/*.hpp")

# add filters to the solution explorer for the shaders :))
file(GLOB SHADER_FILTER   
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/*.vert
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/*.frag
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/*.glsl
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/*.comp
	${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/*.geom
)

file(GLOB PBR_SHADER_FILTER   
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/pbr/*.vert
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/pbr/*.frag
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/pbr/*.glsl
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/pbr/*.comp
	${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/pbr/*.geom
)

file(GLOB BRDF_SHADER_FILTER   
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/pbr/brdf/*.vert
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/pbr/brdf/*.frag
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/pbr/brdf/*.glsl
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/pbr/brdf/*.comp
	${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/pbr/brdf/*.geom
)

file(GLOB COMMON_SHADER_FILTER   
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/common/*.vert
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/common/*.frag
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/common/*.glsl
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/common/*.comp
	${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/common/*.geom
)

file(GLOB CLOUDS_SHADER_FILTER   
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/clouds/*.vert
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/clouds/*.frag
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/clouds/*.glsl
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/clouds/*.comp
	${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/clouds/*.geom
)

file(GLOB LOADING_SHADER_FILTER   
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/loading_screen/*.vert
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/loading_screen/*.frag
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/loading_screen/*.glsl
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/loading_screen/*.comp
	${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/loading_screen/*.geom
)

include(cmake/SetFilters.cmake)

file(GLOB RASTERIZER_SHADOWS_SHADER_FILTER   
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/rasterizer/shadows/*.vert
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/rasterizer/shadows/*.frag
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/rasterizer/shadows/*.glsl
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/rasterizer/shadows/*.comp
	${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/rasterizer/shadows/*.geom
)

source_group("gui"                                      FILES ${GUI_FILTER})
source_group("gui/imgui"                                FILES ${IMGUI_FILTER})

source_group("shaders"                                  FILES ${SHADER_FILTER})
source_group("shaders/hostDevice"                       FILES ${SHADER_HOST_DEVICE_FILTER})
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

source_group("app"                                      FILES ${APP_FILTER})
source_group("common"                                   FILES ${COMMON_FILTER})

#set all your sources accordingly
# file(GLOB_RECURSE SOURCES "Src/*.cpp"
#                           "Src/*.c"
#                           ${CMAKE_CURRENT_SOURCE_DIR}/ExternalLib/GLAD/*.c
#                           ${CMAKE_CURRENT_SOURCE_DIR}/ExternalLib/IMGUI/*.cpp
#                           ${CMAKE_CURRENT_SOURCE_DIR}/ExternalLib/IMGUI/backends/imgui_impl_glfw.cpp
#                           ${CMAKE_CURRENT_SOURCE_DIR}/ExternalLib/IMGUI/backends/imgui_impl_opengl3.cpp)

# file(GLOB_RECURSE HEADERS "Src/*.h"
#                           "Src/*.hpp"
#                           ${CMAKE_CURRENT_SOURCE_DIR}/ExternalLib/GLAD/*.h
#                           ${CMAKE_CURRENT_SOURCE_DIR}/ExternalLib/IMGUI/*.h
#                           ${CMAKE_CURRENT_SOURCE_DIR}/ExternalLib/IMGUI/backends/imgui_impl_glfw.h
#                           ${CMAKE_CURRENT_SOURCE_DIR}/ExternalLib/IMGUI/backends/imgui_impl_opengl3.h)

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
                ${IMGUI_FILTER}
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
                ${COMMON_FILTER}
                ${APP_FILTER})
        
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
                                                PUBLIC "./Resources/Shaders/hostDevice"
                                                ${OPENGL_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME}   PUBLIC  # for imgui
                                                ${CMAKE_DL_LIBS} 
                                                Threads::Threads
                                                ${OPENGL_LIBRARIES}
                                                glfw
                                                imgui
                                                stb 
                                                glm 
                                                tinyobjloader
                                                # enable compiler warnings
                                                project_warnings
                                                # enable sanitizers
                                                project_sanitizers)


target_link_libraries(${PROJECT_NAME}   PRIVATE GSL)

# all testing related stuff
enable_testing()

FILE(GLOB RENDERER_COMPILE_TEST_FILTER          "Test/compile/*.h"                                               "Test/compile/*.cpp"
                                                "Test/compile/*.hpp")

source_group("COMPILE_TEST_FILES"                                   FILES ${RENDERER_COMPILE_TEST_FILTER})

FILE(GLOB RENDERER_COMMIT_TEST_FILTER           "Test/commit/*.h"                                               "Test/commit/*.cpp"
                                                "Test/commit/*.hpp")

source_group("COMMIT_TEST_FILES"                                   FILES ${RENDERER_COMMIT_TEST_FILTER})

set (COMMIT_TEST_SUITE commitTestSuite)
set (COMPILE_TEST_SUITE compileTestSuite)

add_executable(
        ${COMMIT_TEST_SUITE}
        # all test specific files -- start
        ${RENDERER_COMMIT_TEST_FILTER}
        # all test specific files -- end
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
        ${IMGUI_FILTER}
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
        ${COMMON_FILTER}
)

add_executable(
        ${COMPILE_TEST_SUITE}
        # all test specific files -- start
        ${RENDERER_COMPILE_TEST_FILTER}
        # all test specific files -- end
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
        ${IMGUI_FILTER}
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
        ${COMMON_FILTER}
)

target_include_directories(${COMMIT_TEST_SUITE}         PUBLIC "./Src"
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
                                                        PUBLIC "./Resources/Shaders/hostDevice"
                                                        ${OPENGL_INCLUDE_DIRS})

target_include_directories(${COMPILE_TEST_SUITE}        PUBLIC "./Src"
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
                                                        PUBLIC "./Resources/Shaders/hostDevice"
                                                        ${OPENGL_INCLUDE_DIRS})

target_link_libraries(${COMMIT_TEST_SUITE}      PUBLIC  gtest_main
                                                        ${CMAKE_DL_LIBS} # for imgui
                                                        Threads::Threads
                                                        ${OPENGL_LIBRARIES}
                                                        glfw
                                                        imgui
                                                        stb 
                                                        glm 
                                                        tinyobjloader)

target_link_libraries(${COMMIT_TEST_SUITE}      PRIVATE GSL)

target_link_libraries(${COMPILE_TEST_SUITE}      PUBLIC  gtest_main
                                                        ${CMAKE_DL_LIBS} # for imgui
                                                        Threads::Threads
                                                        ${OPENGL_LIBRARIES}
                                                        glfw
                                                        imgui
                                                        stb 
                                                        glm 
                                                        tinyobjloader)

target_link_libraries(${COMPILE_TEST_SUITE}      PRIVATE GSL)

include(GoogleTest)
gtest_discover_tests(   ${COMMIT_TEST_SUITE})
gtest_discover_tests(   ${COMPILE_TEST_SUITE})
# add test after each build
# src: https://quickturnstudio.com/running-test-in-compile-time/
add_custom_target(${COMPILE_TEST_SUITE}_Runner
                  COMMAND ${COMPILE_TEST_SUITE}
                  COMMENT "Running tests after each compilation.")

add_dependencies(${PROJECT_NAME} ${COMPILE_TEST_SUITE}_Runner)

# for correct library output needed
install(TARGETS ${PROJECT_NAME}
        CONFIGURATIONS Release
        RUNTIME DESTINATION bin)

install(DIRECTORY Resources/ DESTINATION Resources/)

# we are using this dir strings for removing hard coded file locations
# at c++ side
configure_file(OpenGLRendererConfig.h.in "${CMAKE_CURRENT_SOURCE_DIR}/Src/renderer/OpenGLRendererConfig.h")

include(InstallRequiredSystemLibraries)

set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")

set(CPACK_SOURCE_IGNORE_FILES
    /.git
    /out
    /.*build.*
)

if(MSVC)
   set(CPACK_SOURCE_GENERATOR "NSIS")
   set(CPACK_NSIS_MUI_ICON ${CMAKE_CURRENT_SOURCE_DIR}/images/faviconNew.ico)
   set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
   set(CPACK_NSIS_HELP_LINK https://jotrockenmitlocken.de/)
   set(CPACK_NSIS_URL_INFO_ABOUT https://jotrockenmitlocken.de/)
   set(CPACK_NSIS_CONTACT https://jotrockenmitlocken.de/)
   set (CPACK_NSIS_MODIFY_PATH "ON")
 else() 
   set(CPACK_SOURCE_GENERATOR "TGZ")
 endif()

include(CPack)