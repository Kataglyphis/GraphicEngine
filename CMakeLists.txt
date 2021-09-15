cmake_minimum_required(VERSION 3.10)

# set the project name
project("GraphicsEngine")

# add the executable
add_executable("${PROJECT_NAME}" "src/PlanetExploration.cpp")

install(TARGETS "${PROJECT_NAME}" DESTINATION bin)
install(FILES "src/PlanetExploration.cpp" DESTINATION src)