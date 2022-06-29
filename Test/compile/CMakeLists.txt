set(WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../)

# update current positions
set(PROJECT_SRC_DIR ${WORKING_DIRECTORY}Src/)
set(EXTERNAL_LIB_SRC_DIR ${WORKING_DIRECTORY}ExternalLib/)
set(SHADER_SRC_DIR ${WORKING_DIRECTORY}Resources/Shaders/)

include(${WORKING_DIRECTORY}cmake/filters/SetShaderFilters.cmake)
include(${WORKING_DIRECTORY}cmake/filters/SetProjectFilters.cmake)
include(${WORKING_DIRECTORY}cmake/filters/SetExternalLibsFilters.cmake)

include(cmake/SetTestFilters.cmake)

set (COMPILE_TEST_SUITE compileTestSuite)

add_executable(
        ${COMPILE_TEST_SUITE}
        # all test specific files -- start
        ${RENDERER_COMPILE_TEST_FILTER}
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

target_include_directories(${COMPILE_TEST_SUITE}        PUBLIC ${WORKING_DIRECTORY}Src
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
                                                        PUBLIC ${OPENGL_INCLUDE_DIRS}) 

target_link_libraries(${COMPILE_TEST_SUITE}      PUBLIC gtest_main
                                                        ${CMAKE_DL_LIBS} # for imgui
                                                        Threads::Threads
                                                        ${OPENGL_LIBRARIES}
                                                        glfw
                                                        imgui
                                                        stb 
                                                        glm 
                                                        tinyobjloader
                                                        glad)

target_link_libraries(${COMPILE_TEST_SUITE}      PRIVATE GSL)

gtest_discover_tests(   ${COMPILE_TEST_SUITE}
                        DISCOVERY_TIMEOUT 300)

# disable all warnings for our test suite
if(MSVC)
  target_compile_options(${COMPILE_TEST_SUITE} INTERFACE /w)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  target_compile_options(${COMPILE_TEST_SUITE} INTERFACE -w)
else()
  target_compile_options(${COMPILE_TEST_SUITE} INTERFACE -w)
endif()

# make our sources available to testing framework
add_custom_target(resourcefilesForCompileTestSuite ALL
                    COMMAND ${CMAKE_COMMAND} -E copy_directory 
                    ${CMAKE_SOURCE_DIR}/Resources 
                    ${CMAKE_BINARY_DIR}/Test/Resources
                    COMMAND ${CMAKE_COMMAND} -E copy_directory 
                    ${CMAKE_SOURCE_DIR}/ExternalLib/IMGUI/misc/fonts/ 
                    ${CMAKE_BINARY_DIR}/Test/ExternalLib/IMGUI/misc/fonts/)

add_dependencies(${COMPILE_TEST_SUITE} resourcefilesForCompileTestSuite)

# add test after each build
# src: https://quickturnstudio.com/running-test-in-compile-time/
add_custom_target(${COMPILE_TEST_SUITE}_Runner
                  COMMAND ${COMPILE_TEST_SUITE}
                  COMMENT "Running tests after each compilation.")

add_dependencies(${PROJECT_NAME} ${COMPILE_TEST_SUITE}_Runner)