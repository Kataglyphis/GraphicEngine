# all testing related stuff

set(WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../)

# update current positions
set(PROJECT_SRC_DIR ${WORKING_DIRECTORY}Src/)
set(EXTERNAL_LIB_SRC_DIR ${WORKING_DIRECTORY}ExternalLib/)
set(SHADER_SRC_DIR ${WORKING_DIRECTORY}Resources/Shaders/)

include(${WORKING_DIRECTORY}cmake/filters/SetShaderFilters.cmake)
include(${WORKING_DIRECTORY}cmake/filters/SetProjectFilters.cmake)
include(${WORKING_DIRECTORY}cmake/filters/SetExternalLibsFilters.cmake)

include(cmake/SetTestFilters.cmake)

set (COMMIT_TEST_SUITE commitTestSuite)

add_executable(
        ${COMMIT_TEST_SUITE}
        # all test specific files -- start
        ${RENDERER_COMMIT_TEST_FILTER}
        # all test specific files -- end
        $<TARGET_OBJECTS:PROJECT_FILES_TO_TEST>
        $<TARGET_OBJECTS:IMGUI>
        $<TARGET_OBJECTS:GLAD>
)

target_include_directories(${COMMIT_TEST_SUITE}         PUBLIC ${WORKING_DIRECTORY}Src
                                                        PUBLIC ${WORKING_DIRECTORY}Src/camera
                                                        PUBLIC ${WORKING_DIRECTORY}Src/compute
                                                        PUBLIC ${WORKING_DIRECTORY}Src/renderer
                                                        PUBLIC ${WORKING_DIRECTORY}Src/renderer/loading_screen
                                                        PUBLIC ${WORKING_DIRECTORY}Src/renderer/deferred
                                                        PUBLIC ${WORKING_DIRECTORY}Src/debug
                                                        PUBLIC ${WORKING_DIRECTORY}Src/window
                                                        PUBLIC ${WORKING_DIRECTORY}Src/scene
                                                        PUBLIC ${WORKING_DIRECTORY}Src/scene/atmospheric_effects/clouds
                                                        PUBLIC ${WORKING_DIRECTORY}Src/scene/light
                                                        PUBLIC ${WORKING_DIRECTORY}Src/scene/light/directional_light
                                                        PUBLIC ${WORKING_DIRECTORY}Src/scene/light/point_light
                                                        PUBLIC ${WORKING_DIRECTORY}Src/scene/shadows
                                                        PUBLIC ${WORKING_DIRECTORY}Src/scene/sky_box
                                                        PUBLIC ${WORKING_DIRECTORY}Src/scene/texture
                                                        PUBLIC ${WORKING_DIRECTORY}Src/gui
                                                        PUBLIC ${WORKING_DIRECTORY}Src/util
                                                        PUBLIC ${WORKING_DIRECTORY}Resources/Shaders/hostDevice
                                                        ${OPENGL_INCLUDE_DIRS})

target_include_directories(${COMMIT_TEST_SUITE} PRIVATE ${OPENGL_INCLUDE_DIRS})

target_link_libraries(${COMMIT_TEST_SUITE}      PUBLIC  gtest_main
                                                        ${CMAKE_DL_LIBS} # for imgui
                                                        Threads::Threads
                                                        ${OPENGL_LIBRARIES}
                                                        glfw
                                                        imgui
                                                        stb 
                                                        glm 
                                                        tinyobjloader
                                                        glad)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  include(cmake/CodeCoverage.cmake)
  option(ENABLE_COVERAGE "Enable coverage reporting for gcc/clang" ON)
  if(ENABLE_COVERAGE)
    target_compile_options(${COMMIT_TEST_SUITE}  INTERFACE -O0)
    include(CodeCoverage)
    append_coverage_compiler_flags_to_target(${COMMIT_TEST_SUITE})
    setup_target_for_coverage_gcovr_html(
            NAME coverage
            EXECUTABLE testrunner
            # BASE_DIRECTORY "${PROJECT_SOURCE_DIR}"
            EXCLUDE "ExternalLib/*")
      
  endif()

endif()

# disable all warnings for our test suite
if(MSVC)
  target_compile_options(${COMMIT_TEST_SUITE}  INTERFACE /w)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  target_compile_options(${COMMIT_TEST_SUITE}  INTERFACE -w)
else()
  target_compile_options(${COMMIT_TEST_SUITE}  INTERFACE -w)
endif()

include(GoogleTest)
gtest_discover_tests(   ${COMMIT_TEST_SUITE})