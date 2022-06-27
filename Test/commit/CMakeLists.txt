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

target_link_libraries(${COMMIT_TEST_SUITE}   PRIVATE GSL)

# good source: https://stackoverflow.com/questions/52255589/code-coverage-analysis-using-codecoverage-cmake-with-multiple-targets
option(COVERAGE "Enable Code Coverage" ON)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  if(COVERAGE)
  include(cmake/CodeCoverage.cmake)
  append_coverage_compiler_flags()
  setup_target_for_coverage_gcovr_xml(
          NAME coverage_xml                 
          EXECUTABLE ctest -j ${n_cores} # Executable in PROJECT_BINARY_DIR
          EXCLUDE   "ExternalLib/*"
                    "build/*"
                    "Documents/*"
  )
  setup_target_for_coverage_gcovr_html(
          NAME coverage_html                 
          EXECUTABLE ctest -j ${n_cores} # Executable in PROJECT_BINARY_DIR
          EXCLUDE   "ExternalLib/*"
                    "build/*"
                    "Documents/*"
  )
  
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