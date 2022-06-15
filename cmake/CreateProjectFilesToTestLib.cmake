# how to exclude files from static code analysis
# https://stackoverflow.com/questions/49591804/clang-tidy-cmake-exclude-file-from-check/49591908#49591908
# here we create a library with all specific render agnostic code so we can test it without static code analysis
add_library(
    PROJECT_FILES_TO_TEST
    OBJECT
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
)

target_include_directories(  PROJECT_FILES_TO_TEST      PUBLIC Src
                                                        PUBLIC Src/camera
                                                        PUBLIC Src/compute
                                                        PUBLIC Src/renderer
                                                        PUBLIC Src/renderer/loading_screen
                                                        PUBLIC Src/renderer/deferred
                                                        PUBLIC Src/debug
                                                        PUBLIC Src/window
                                                        PUBLIC Src/scene
                                                        PUBLIC Src/scene/atmospheric_effects/clouds
                                                        PUBLIC Src/scene/light
                                                        PUBLIC Src/scene/light/directional_light
                                                        PUBLIC Src/scene/light/point_light
                                                        PUBLIC Src/scene/shadows
                                                        PUBLIC Src/scene/sky_box
                                                        PUBLIC Src/scene/texture
                                                        PUBLIC Src/gui
                                                        PUBLIC Src/util
                                                        PUBLIC ExternalLib/GLAD/include
                                                        PUBLIC Resources/Shaders/hostDevice
)

target_include_directories(  PROJECT_FILES_TO_TEST PRIVATE    ${OPENGL_INCLUDE_DIRS})

target_link_libraries(PROJECT_FILES_TO_TEST   PRIVATE   gtest_main
                                                        ${CMAKE_DL_LIBS} # for imgui
                                                        Threads::Threads
                                                        ${OPENGL_LIBRARIES}
                                                        glfw
                                                        imgui
                                                        stb 
                                                        glm 
                                                        tinyobjloader
)

target_link_libraries(PROJECT_FILES_TO_TEST   PRIVATE GSL)

# disable all warnings for our test suite
if(MSVC)
  target_compile_options(PROJECT_FILES_TO_TEST INTERFACE /w)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  target_compile_options(PROJECT_FILES_TO_TEST INTERFACE -w)
else()
  target_compile_options(PROJECT_FILES_TO_TEST INTERFACE -w)
endif()

set_target_properties(
    PROJECT_FILES_TO_TEST
    PROPERTIES
    CXX_CLANG_TIDY ""
    CXX_CPPCHECK ""
)