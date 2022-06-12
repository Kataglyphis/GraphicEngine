# setting all project filters
# ---- EXTERNAL LIBS FILTER  --- BEGIN

# ---- GUI FILTER  --- BEGIN
set(EXTERNAL_LIB_GUI_SRC_DIR ${EXTERNAL_LIB_SRC_DIR}IMGUI/)
set(IMGUI_FILTER ${IMGUI_FILTER}
    ${EXTERNAL_LIB_GUI_SRC_DIR}imconfig.h
    ${EXTERNAL_LIB_GUI_SRC_DIR}imgui.cpp
    ${EXTERNAL_LIB_GUI_SRC_DIR}imgui.h
    ${EXTERNAL_LIB_GUI_SRC_DIR}imgui_demo.cpp
    ${EXTERNAL_LIB_GUI_SRC_DIR}imgui_draw.cpp
    ${EXTERNAL_LIB_GUI_SRC_DIR}imgui_internal.h
    ${EXTERNAL_LIB_GUI_SRC_DIR}imgui_tables.cpp
    ${EXTERNAL_LIB_GUI_SRC_DIR}imgui_widgets.cpp
    ${EXTERNAL_LIB_GUI_SRC_DIR}imstb_rectpack.h
    ${EXTERNAL_LIB_GUI_SRC_DIR}imstb_textedit.h
    ${EXTERNAL_LIB_GUI_SRC_DIR}imstb_truetype.h
    ${EXTERNAL_LIB_GUI_SRC_DIR}backends/imgui_impl_glfw.h
    ${EXTERNAL_LIB_GUI_SRC_DIR}backends/imgui_impl_glfw.cpp
    ${EXTERNAL_LIB_GUI_SRC_DIR}backends/imgui_impl_opengl3.h
    ${EXTERNAL_LIB_GUI_SRC_DIR}backends/imgui_impl_opengl3.cpp
)
# ---- GUI FILTER  --- END

# ---- GLAD FILTER  --- BEGIN
set(EXTERNAL_LIB_GLAD_SRC_DIR ${EXTERNAL_LIB_SRC_DIR}GLAD/)
set(GLAD_FILTER ${GLAD_FILTER}
    ${EXTERNAL_LIB_GLAD_SRC_DIR}include/glad/glad.h
    ${EXTERNAL_LIB_GLAD_SRC_DIR}include/KHR/khrplatform.h
    ${EXTERNAL_LIB_GLAD_SRC_DIR}src/glad.c
)
# ---- GLAD FILTER  --- END