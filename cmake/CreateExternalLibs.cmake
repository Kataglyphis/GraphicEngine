# IMGUI LIB
add_library(
    IMGUI
    OBJECT
    ${IMGUI_FILTER}
)

target_link_libraries(IMGUI         PRIVATE ${CMAKE_DL_LIBS}
                                            ${OPENGL_LIBRARIES}
                                            glfw 
                                            imgui

)

target_include_directories(IMGUI    PRIVATE ${OPENGL_INCLUDE_DIRS})

set_target_properties(
    IMGUI
    PROPERTIES
    CXX_CLANG_TIDY ""
    CXX_CPPCHECK ""
)

# GLAD LIB
add_library(
    GLAD
    OBJECT
    ${GLAD_FILTER}
)

target_link_libraries(GLAD         PRIVATE ${OPENGL_LIBRARIES}

)

target_include_directories(GLAD    PRIVATE ./ExternalLib/GLAD/include
                                            ${OPENGL_INCLUDE_DIRS})

set_target_properties(
    GLAD
    PROPERTIES
    CXX_CLANG_TIDY ""
    CXX_CPPCHECK ""
)