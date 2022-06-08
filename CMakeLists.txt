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

# add compile flags; as restrictive as possible
# for windows
# source: https://github.com/cpp-best-practices/cppbestpractices/blob/master/02-Use_the_Tools_Available.md
set(MSVC_WARNINGS
          /W4 # All reasonable warnings
          /w14242  #'identfier': conversion from 'type1' to 'type1', possible loss of data
          /w14254  #'operator': conversion from 'type1:field_bits' to 'type2:field_bits', possible loss of data
          /w14263  #'function': member function does not override any base class virtual member function
          /w14265  #'classname': class has virtual functions, but destructor is not virtual instances of this class may not be destructed correctly
          /w14287  #'operator': unsigned/negative constant mismatch
          /we4289  #nonstandard extension used: 'variable': loop control variable declared in the for-loop is used outside the for-loop scope
          /w14296  #'operator': expression is always 'boolean_value'
          /w14311  #'variable': pointer truncation from 'type1' to 'type2'
          /w14545  #expression before comma evaluates to a function which is missing an argument list
          /w14546  #function call before comma missing argument list
          /w14547  #'operator': operator before comma has no effect; expected operator with side-effect
          /w14549  #'operator': operator before comma has no effect; did you intend 'operator'?
          /w14555  #expression has no effect; expected expression with side-effect
          /w14619  #pragma warning: there is no warning number 'number'
          /w14640  #Enable warning on thread un-safe static member initialization
          /w14826  #Conversion from 'type1' to 'type_2' is sign-extended. This may cause unexpected runtime behavior.
          /w14905  #wide string literal cast to 'LPSTR'
          /w14906  #string literal cast to 'LPWSTR'
          /w14928  #illegal copy-initialization; more than one user-defined conversion has been implicitly applied
)

# CLANG
set(CLANG_WARNINGS
          -pedantic -Warn # on language extensions
          -Wall -Wextra # reasonable and standard
          -Wshadow # warn the user if a variable declaration shadows one from a parent context
          -Wnon-virtual-dtor # warn the user if a class with virtual functions has a non-virtual destructor. This helps catch hard to track down memory errors
          -Wold-style-cast # warn for c-style casts
          -Wcast-align # warn for potential performance problem casts
          -Wunused # warn on anything being unused
          -Woverloaded-virtual # warn if you overload (not override) a virtual function
          -Wpedantic # (all versions of GCC, Clang >= 3.2) warn if non-standard C++ is used
          -Wconversion # warn on type conversions that may lose data
          -Wsign-conversion # (Clang all versions, GCC >= 4.3) warn on sign conversions
          -Wmisleading-indentation # (only in GCC >= 6.0) warn if indentation implies blocks where blocks do not exist
          -Wduplicated-cond # (only in GCC >= 6.0) warn if if / else chain has duplicated conditions
          -Wduplicated-branches # (only in GCC >= 7.0) warn if if / else branches have duplicated code
          -Wlogical-op # (only in GCC) warn about logical operations being used where bitwise were probably wanted
          -Wnull-dereference # (only in GCC >= 6.0) warn if a null dereference is detected
          -Wuseless-cast # (only in GCC >= 4.8) warn if you perform a cast to the same type
          -Wdouble-promotion # (GCC >= 4.6, Clang >= 3.8) warn if float is implicit promoted to double
          -Wformat=2 # warn on security issues around functions that format output (ie printf)
          -Wlifetime #(only special branch of Clang currently) shows object lifetime issues
)

option(TREAT_WARNINGS_AS_ERRORS "Treat warnings as errors" ON)
if(TREAT_WARNINGS_AS_ERRORS)
  set(CLANG_WARNINGS ${CLANG_WARNINGS} -Werror)
  set(MSVC_WARNINGS ${MSVC_WARNINGS} /WX)
endif()

# GCC 
set(GCC_WARNINGS ${CLANG_WARNINGS})

if(MSVC)
  set(PROJECT_WARNINGS ${MSVC_WARNINGS})
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(PROJECT_WARNINGS ${CLANG_WARNINGS})
else()
  set(PROJECT_WARNINGS ${GCC_WARNINGS})
endif()

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
find_package(Threads REQUIRED)

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

file(GLOB RASTERIZER_SHADER_FILTER   
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/rasterizer/*.vert
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/rasterizer/*.frag
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/rasterizer/*.glsl
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/rasterizer/*.comp
	${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/rasterizer/*.geom
)

file(GLOB RASTERIZER_SHADOWS_SHADER_FILTER   
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/rasterizer/shadows/*.vert
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/rasterizer/shadows/*.frag
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/rasterizer/shadows/*.glsl
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/rasterizer/shadows/*.comp
	${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/rasterizer/shadows/*.geom
)

file(GLOB SKYBOX_SHADER_FILTER   
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/skybox/*.vert
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/skybox/*.frag
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/skybox/*.glsl
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/skybox/*.comp
	${CMAKE_CURRENT_SOURCE_DIR}/Resources/Shaders/skybox/*.geom
)

source_group("gui"                                      FILES ${GUI_FILTER})
source_group("gui/imgui"                                FILES ${IMGUI_FILTER})

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

source_group("app"                                      FILES ${APP_FILTER})
source_group("common"                                   FILES ${COMMON_FILTER})

#set all your sources accordingly
file(GLOB_RECURSE SOURCES "Src/*.cpp"
                          "Src/*.c"
                          ${CMAKE_CURRENT_SOURCE_DIR}/ExternalLib/GLAD/*.c
                          ${CMAKE_CURRENT_SOURCE_DIR}/ExternalLib/IMGUI/*.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/ExternalLib/IMGUI/backends/imgui_impl_glfw.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/ExternalLib/IMGUI/backends/imgui_impl_opengl3.cpp)

file(GLOB_RECURSE HEADERS "Src/*.h"
                          "Src/*.hpp"
                          ${CMAKE_CURRENT_SOURCE_DIR}/ExternalLib/GLAD/*.h
                          ${CMAKE_CURRENT_SOURCE_DIR}/ExternalLib/IMGUI/*.h
                          ${CMAKE_CURRENT_SOURCE_DIR}/ExternalLib/IMGUI/backends/imgui_impl_glfw.h
                          ${CMAKE_CURRENT_SOURCE_DIR}/ExternalLib/IMGUI/backends/imgui_impl_opengl3.h)

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

target_compile_options(${PROJECT_NAME} INTERFACE ${PROJECT_WARNINGS})
        
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


target_link_libraries(${PROJECT_NAME}   PRIVATE GSL)

set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "${GraphicsEngine_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${GraphicsEngine_VERSION_MINOR}")

# we really need this; glew already build its libraries in bin/ folder
# otherwise program will not find our library!
set_target_properties( ${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                                    ${PROJECT_BINARY_DIR})

set_target_properties( ${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY
                       ${PROJECT_BINARY_DIR})

# first we can indicate the documentation build as an option and set it to ON by default
option(BUILD_DOC "Build documentation" ON)

# check if Doxygen is installed
if(BUILD_DOC)
        find_package(Doxygen)
        if (DOXYGEN_FOUND)
        # set input and output files
        set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
        set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)    # request to configure the file
        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
        message("Doxygen build started")    # note the option ALL which allows to build the docs together with the application
        add_custom_target( doc_doxygen ALL
                COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                COMMENT "Generating API documentation with Doxygen"
                VERBATIM )
        else (DOXYGEN_FOUND)
        message("Doxygen need to be installed to generate the doxygen documentation")
        endif (DOXYGEN_FOUND)
endif()

option(ENABLE_CPPCHECK "Enable cppcheck" OFF)#ON
if(ENABLE_CPPCHECK)
        find_program(CPPCHECK cppcheck)
        if(CPPCHECK)
        set(CMAKE_CXX_CPPCHECK ${CPPCHECK} --suppress=missingInclude --enable=all --suppressions-list=Documents/cppcheck/suppressions.txt)
        endif(CPPCHECK)
endif()

option(ENABLE_CLANGTIDY "Enable clangtidy" OFF)#ON
if(ENABLE_CLANGTIDY)
        find_program(CLANGTIDY clang-tidy)
        if(CLANGTIDY)
        set(CMAKE_CXX_CLANG_TIDY ${CLANGTIDY})
        endif(CLANGTIDY)
endif()

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
gtest_discover_tests(   ${COMMIT_TEST_SUITE} ${COMPILE_TEST_SUITE})# ${CMAKE_CURRENT_SOURCE_DIR}/Tests

# add test after each build
# src: https://quickturnstudio.com/running-test-in-compile-time/
add_custom_target(${COMPILE_TEST_SUITE}_Runner
                  COMMAND ${COMPILE_TEST_SUITE}
                  COMMENT "Running tests after each compilation.")

add_dependencies(${PROJECT_NAME} ${COMPILE_TEST_SUITE}_Runner)


include(CPack)