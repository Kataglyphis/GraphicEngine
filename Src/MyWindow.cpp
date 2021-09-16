#include "MyWindow.h"

MyWindow::MyWindow() {
    //at least sth useful
    window_width = 800;
    window_height = 600;

    // all keys non-pressed in the beginning
    for (size_t i = 0; i < 1024; i++) {
        keys[i] = 0;
    }
    x_change = 0.0f;
    y_change = 0.0f;
}

//please use this constructor; never the standard
MyWindow::MyWindow(GLint window_width, GLint window_height) {
    this->window_width = window_width;
    this->window_height = window_height;

    // all keys non-pressed in the beginning
    for (size_t i = 0; i < 1024; i++) {
        keys[i] = 0;
    }
    x_change = 0.0f;
    y_change = 0.0f;
}

int MyWindow::initialize() {
    
    if (!glfwInit()) {

        printf("GLFW Init failed!");
        glfwTerminate();
        return 1;

    }

    // setup glfw window properties
    
    //lets work with nothing older than version 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // core profile = no backward compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // allow forward compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    //allow it to resize
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    //retrieve new window
    main_window = glfwCreateWindow(window_width, window_height, "Planet Exploration", NULL, NULL);

    if (!main_window) {

        printf("GLFW Window creation failed!");
        glfwTerminate();
        return 1;

    }

    // get buffer size information
    glfwGetFramebufferSize(main_window, &window_buffer_width, &window_buffer_height);

    // set context for GLEW to use
    glfwMakeContextCurrent(main_window);

    //Handle key + mouse Input
    //TODO: here we need to further go on and implement game logic
    init_callbacks();
    glfwSetInputMode(main_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // allow modern extension features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {

        printf("GLEW Init failed!");
        glfwDestroyWindow(main_window);
        glfwTerminate();
        return 1;

    }

    glEnable(GL_DEPTH_TEST);

    // setup viewport size
    glViewport(0, 0, window_buffer_width, window_buffer_height);

    glfwSetWindowUserPointer(main_window, this);

    return 0;
}

void MyWindow::update_viewport() {

    glfwGetFramebufferSize(main_window, &window_buffer_width, &window_buffer_height);
    // setup viewport size
    glViewport(0, 0, window_buffer_width, window_buffer_height);

}

void MyWindow::set_buffer_size(GLfloat window_buffer_width, GLfloat window_buffer_height) {
    this->window_buffer_width = window_buffer_width;
    this->window_buffer_height = window_buffer_height;
}

GLfloat MyWindow::get_x_change()
{
    GLfloat the_change = x_change;
    x_change = 0.0f;
    return the_change;
}

GLfloat MyWindow::get_y_change()
{
    GLfloat the_change = y_change;
    y_change = 0.0f;
    return the_change;
}

MyWindow::~MyWindow()
{
    glfwDestroyWindow(main_window);
    glfwTerminate();
}

void MyWindow::init_callbacks()
{
    //TODO: remember this section for our later game logic 
    //for the space ship to fly around
    glfwSetKeyCallback(main_window, key_callback);
    glfwSetMouseButtonCallback(main_window, mouse_button_callback);
    glfwSetFramebufferSizeCallback(main_window, framebuffer_size_callback);

}

void MyWindow::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
}

void MyWindow::key_callback(GLFWwindow* window, int key, int code, int action, int mode)
{
    MyWindow* the_window = static_cast<MyWindow*>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            the_window->keys[key] = true;

        }
        else if (action == GLFW_RELEASE) {
            the_window->keys[key] = false;

        }
    }
}

void MyWindow::mouse_callback(GLFWwindow* window, double x_pos, double y_pos)
{
    MyWindow* the_window = static_cast<MyWindow*>(glfwGetWindowUserPointer(window));

    // need to handle first occurance of a mouse moving event
    if (the_window->mouse_first_moved) {
        the_window->last_x = (float)x_pos;
        the_window->last_y = (float)y_pos;
        the_window->mouse_first_moved = false;
    }

    the_window->x_change = (float)(x_pos - the_window->last_x);
    // take care of correct substraction :)
    the_window->y_change = (float)(the_window->last_y - y_pos);

    //update params
    the_window->last_x = (float)x_pos;
    the_window->last_y = (float)y_pos;
}

void MyWindow::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    MyWindow* the_window = static_cast<MyWindow*>(glfwGetWindowUserPointer(window));

    if ((action == GLFW_PRESS) && (button == GLFW_MOUSE_BUTTON_RIGHT)) {
        glfwSetCursorPosCallback(window, mouse_callback);
    }
    else {
        the_window->mouse_first_moved = true;
        glfwSetCursorPosCallback(window, NULL);
    }
}
