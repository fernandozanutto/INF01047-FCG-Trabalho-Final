#pragma once
   
#include <GLFW/glfw3.h>


class Window {
//private:
public:
    GLFWwindow* window;
    float screenRatio;

    unsigned int width;
    unsigned int height;

    Window();
    ~Window();

    //void setKeyCallbacks(InputManager* input);
    bool shouldClose();
    void setShouldClose();
    void swapBuffers();
    void pollEvents();
};