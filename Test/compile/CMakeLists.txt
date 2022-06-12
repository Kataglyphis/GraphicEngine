# update current positions
set(PROJECT_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../Src/)
set(EXTERNAL_LIB_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../ExternalLib/)
set(SHADER_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../Resources/Shaders/)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/filters/SetShaderFilters.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/filters/SetProjectFilters.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/filters/SetExternalLibsFilters.cmake)

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

target_include_directories(${COMPILE_TEST_SUITE}        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Src
                                                        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Src/camera
                                                        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Src/compute
                                                        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Src/renderer
                                                        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Src/renderer/loading_screen
                                                        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Src/renderer/deferred
                                                        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Src/debug
                                                        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Src/window
                                                        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Src/scene
                                                        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Src/scene/atmospheric_effects/clouds
                                                        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Src/scene/light
                                                        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Src/scene/light/directional_light
                                                        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Src/scene/light/point_light
                                                        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Src/scene/shadows
                                                        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Src/scene/sky_box
                                                        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Src/scene/texture
                                                        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Src/gui
                                                        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Src/util
                                                        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../ExternalLib/GLAD/include
                                                        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Resources/Shaders/hostDevice
                                                        ${OPENGL_INCLUDE_DIRS})

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
gtest_discover_tests(   ${COMPILE_TEST_SUITE})

# add test after each build
# src: https://quickturnstudio.com/running-test-in-compile-time/
add_custom_target(${COMPILE_TEST_SUITE}_Runner
                  COMMAND ${COMPILE_TEST_SUITE}
                  COMMENT "Running tests after each compilation.")

add_dependencies(${PROJECT_NAME} ${COMPILE_TEST_SUITE}_Runner)