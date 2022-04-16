#include <cstdlib>
#include <cstdio>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Window.h"

Window::Window() {
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: glfwInit() failed.\n");
        std::exit(EXIT_FAILURE);
    }
    
    glfwSetErrorCallback([](int error, const char* description) {
        fprintf(stderr, "ERROR: GLFW: %s\n", description);
    });
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    //glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* screen = glfwGetVideoMode(monitor);
    this->window = glfwCreateWindow(screen->width, screen->height, "Super Game", monitor, NULL);
    this->width = screen->width;
    this->height = screen->height;

    if (!this->window) {
        glfwTerminate();
        fprintf(stderr, "ERROR: glfwCreateWindow() failed.\n");
        std::exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(this->window);

    glfwSetFramebufferSizeCallback(this->window, [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    });


    //if possible, uses raw input, for better aim control
    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(this->window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    

    //hides mouse cursor
    glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
}

Window::~Window() {
   glfwDestroyWindow(this->window); 
   glfwTerminate();
}

bool Window::shouldClose() {
    return glfwWindowShouldClose(this->window);
}

void Window::setShouldClose() {
    glfwSetWindowShouldClose(this->window, GL_TRUE);
}

void Window::swapBuffers() {
    glfwSwapBuffers(this->window);
}

void Window::pollEvents() {
    glfwPollEvents();
}