cmake_minimum_required(VERSION 3.10)

# set the project name
project(GraphicsEngine VERSION 1.0
                       DESCRIPTION "Epic Graphics"
                       LANGUAGES CXX)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# control where the static and shared libraries are built so that on windows
# we don't need to tinker with the path to run the executable
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

# configure a header file to pass the version number only
configure_file(src/Config.h.in src/Config.h)

#set all your sources accordingly
file(GLOB SOURCES "src/*")

# add the executable
add_executable(${PROJECT_NAME} ${SOURCES})


# include the normal header file 
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}/../src" 
                                           PUBLIC "${PROJECT_BINARY_DIR}/../ExternalLib/GLM"
                                           PUBLIC "${PROJECT_BINARY_DIR}/../ExternalLib/IMGUI"
                                           PUBLIC "${PROJECT_BINARY_DIR}/../ExternalLib/STB"
                                           PUBLIC "${PROJECT_BINARY_DIR}/../ExternalLib/TOL"
                                           PUBLIC "${PROJECT_BINARY_DIR}/../ExternalLib/IMGUI/backends"
                                           PUBLIC "${PROJECT_BINARY_DIR}/../ExternalLib/GLEW/include/GL"
                                           PUBLIC "${PROJECT_BINARY_DIR}/../ExternalLib/GLEW/include")

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

install(FILES "${PROJECT_BINARY_DIR}/src/Config.h"
  DESTINATION include)

install(FILES src/PlanetExploration.cpp DESTINATION src)

# we depend on OpenGL
find_package( OpenGL REQUIRED )

# set all glfw dependencies
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(ExternalLib/GLFW/glfw-3.3.4)
target_link_libraries(${PROJECT_NAME} glfw)

# set all glew dependencies
add_subdirectory(ExternalLib/GLEW)
target_link_libraries(${PROJECT_NAME} glew)

# add tests 
include(CTest)

add_test(test1 ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME})

set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "${GraphicsEngine_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${GraphicsEngine_VERSION_MINOR}")
include(CPack)