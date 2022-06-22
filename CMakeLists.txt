cmake_minimum_required(VERSION 3.20)

# set the project name
project(GraphicsEngine VERSION 1.9
                       DESCRIPTION "Epic Graphics"
                       HOMEPAGE_URL "https://jotrockenmitlocken.de/"
                       LANGUAGES CXX C)

set(AUTHOR "Jotrockenmitlocken")
set(CMAKE_BUILD_TYPE Debug)

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

add_library(ipo INTERFACE)
include(cmake/Optimizations.cmake)
enable_ipo_for_target(ipo)

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
# set all glfw dependencies
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(ExternalLib)

# we depend on OpenGL
find_package( OpenGL REQUIRED COMPONENTS OpenGL)
# configure OpenGL version
set(OPENGL_VERSION_MAJOR 4)
set(OPENGL_VERSION_MINOR 6)
set(OpenGL_GL_PREFERENCE GLVND)

# for faster building
find_package(Threads REQUIRED)

# https://stackoverflow.com/questions/1027247/is-it-better-to-specify-source-files-with-glob-or-each-file-individually-in-cmak
# https://stackoverflow.com/questions/45090926/cmake-generate-list-of-source-files-without-glob
# lets remove the FILE GLOB ... it is bad (as stated in the docs ... https://cmake.org/cmake/help/latest/command/file.html)

# update current positions
set(PROJECT_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Src/)
set(EXTERNAL_LIB_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ExternalLib/)
set(SHADER_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/)

include(cmake/filters/SetShaderFilters.cmake)
include(cmake/filters/SetProjectFilters.cmake)
include(cmake/filters/SetExternalLibsFilters.cmake)

include(cmake/SetSourceGroups.cmake)

# https://stackoverflow.com/questions/49591804/clang-tidy-cmake-exclude-file-from-check/49591908#49591908
# with this trick IMGUI will be excluded from time consuming static code analysis
# we do not want to analyze imgui ...
include(cmake/CreateProjectFilesToTestLib.cmake)
# and we will not to all this for our testing executable again :)
include(cmake/CreateExternalLibs.cmake)

# add the executable
add_executable( ${PROJECT_NAME} 
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
                # this is great; no CPPCHECK,CLANG_TIDY here
                $<TARGET_OBJECTS:IMGUI>
                $<TARGET_OBJECTS:GLAD>
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

enable_cppcheck(${PROJECT_NAME})
enable_clangtidy(${PROJECT_NAME})
  
target_include_directories(${PROJECT_NAME}      PUBLIC ./Src
                                                PUBLIC ./Src/camera
                                                PUBLIC ./Src/compute
                                                PUBLIC ./Src/renderer
                                                PUBLIC ./Src/renderer/loading_screen
                                                PUBLIC ./Src/renderer/deferred
                                                PUBLIC ./Src/debug
                                                PUBLIC ./Src/window
                                                PUBLIC ./Src/scene
                                                PUBLIC ./Src/scene/atmospheric_effects/clouds
                                                PUBLIC ./Src/scene/light
                                                PUBLIC ./Src/scene/light/directional_light
                                                PUBLIC ./Src/scene/light/point_light
                                                PUBLIC ./Src/scene/shadows
                                                PUBLIC ./Src/scene/sky_box
                                                PUBLIC ./Src/scene/texture
                                                PUBLIC ./Src/gui
                                                PUBLIC ./Src/util
                                                PUBLIC ./Resources/Shaders/hostDevice
                          )

target_include_directories(${PROJECT_NAME}      PRIVATE ${OPENGL_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME}   PUBLIC  # for imgui
                                                ${CMAKE_DL_LIBS} 
                                                Threads::Threads
                                                ${OPENGL_LIBRARIES}
                                                glfw
                                                imgui
                                                stb 
                                                glm 
                                                tinyobjloader
                                                glad
                                                # enable compiler warnings
                                                project_warnings
                                                # enable sanitizers
                                                project_sanitizers
                                                # enable ipo
                                                ipo)


target_link_libraries(${PROJECT_NAME}   PRIVATE GSL)

# all testing related stuff
enable_testing()

add_subdirectory(Test/commit)
add_subdirectory(Test/compile)

# for correct library output needed
install(TARGETS ${PROJECT_NAME}
        CONFIGURATIONS Release
        RUNTIME DESTINATION bin)

install(DIRECTORY Resources/ DESTINATION Resources/)

# we are using this dir strings for removing hard coded file locations
# at c++ side
configure_file(OpenGLRendererConfig.hpp.in "${CMAKE_CURRENT_SOURCE_DIR}/Src/renderer/OpenGLRendererConfig.hpp")

include(InstallRequiredSystemLibraries)

include(cmake/CPackOptions.cmake)

include(CPack)