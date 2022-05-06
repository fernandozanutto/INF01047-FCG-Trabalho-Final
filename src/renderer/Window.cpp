#include <cstdlib>
#include <cstdio>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Window.h"
#include "../input/InputManager.h"

Window::Window() {
    bool isFullscreen = false;

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

    //https://www.glfw.org/docs/latest/monitor_guide.html#monitor_monitors
    int monitorCount;
    GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
    GLFWmonitor* monitor = monitors[monitorCount - 1];

    std::cout << "Monitores detectados: " << monitorCount << std::endl;

    const GLFWvidmode* screen = glfwGetVideoMode(monitor);

    if (isFullscreen) {
        this->window = glfwCreateWindow(screen->width, screen->height, "Super Game", monitor, NULL);
    } else {
        this->window = glfwCreateWindow(screen->width, screen->height, "Super Game", NULL, NULL);
    }

    this->width = screen->width;
    this->height = screen->height;

    if (!this->window) {
        fprintf(stderr, "ERROR: glfwCreateWindow() failed.\n");
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(this->window);

    glfwSetFramebufferSizeCallback(this->window, [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    });

    //if possible, uses raw input, for better aim control
    if (glfwRawMouseMotionSupported()) {
        glfwSetInputMode(this->window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }

    //hides mouse cursor
    glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
}

Window::~Window() {
   glfwDestroyWindow(this->window);
   glfwTerminate();
}

void Window::setKeyCallbacks(InputManager* input) {
    // para que possamos pegar a referencia do InputManager de dentro dos callbacks
    // https://code-examples.net/en/q/75242b
    glfwSetWindowUserPointer(this->window, input);

    glfwSetKeyCallback(this->window, [](GLFWwindow* window, int key, int scancode, int action, int mod) {
        InputManager* input = (InputManager*) glfwGetWindowUserPointer(window);
        input->keyCallback(key, action, mod);
    });
    //lambda used for callbacks (calls input manager for handling)

    glfwSetMouseButtonCallback(this->window, [](GLFWwindow* window, int button, int action, int mod) {
        InputManager* input = (InputManager*) glfwGetWindowUserPointer(window);
        input->mouseButtonCallback(button, action, mod);
    });

    glfwSetCursorPosCallback(this->window, [](GLFWwindow* window, double xpos, double ypos) {
        InputManager* input = (InputManager*) glfwGetWindowUserPointer(window);
        input->cursorCallback(xpos, ypos);
    });

    //sets initial position for mouse, since movement is calculated from mouse displacement
    double x, y;
    glfwGetCursorPos(this->window, &x, &y);
    input->setInitialCursorPos(x, y);
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

    if (game != NULL && game->hasFinished) {
        setShouldClose();
    }
}

void Window::setGame(Game* game) {
    this->game = game;
}
