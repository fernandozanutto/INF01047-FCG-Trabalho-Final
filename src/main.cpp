#include <cmath>
#include <cstdio>
#include <cstdlib>

// Headers abaixo são específicos de C++
#include <algorithm>
#include <fstream>
#include <limits>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <iostream>

// Headers das bibliotecas OpenGL
#include <glad/glad.h>   // Criação de contexto OpenGL 3.3, precisa importar antes do glfw3
#include <GLFW/glfw3.h>  // Criação de janelas do sistema operacional


// Headers da biblioteca GLM: criação de matrizes e vetores.
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

// Headers da biblioteca para carregar modelos obj
#include <stb_image.h>
#include <tiny_obj_loader.h>

// Headers locais, definidos na pasta "include/"
#include "utils.h"

#include "renderer/Model.h"
#include "renderer/matrices.h"
#include "renderer/Window.h"
#include "renderer/Renderer.h"

#include "input/Command.h"
#include "input/MoveCommand.h"
#include "input/EscCommand.h"
#include "input/PrimaryActionCommand.h"
#include "input/SecondaryActionCommand.h"
#include "input/ReloadShadersCommand.h"
#include "input/InputManager.h"
#include "input/RotateTarget.h"

#include "game/FirstScene.h"
#include "game/Game.h"

// Declaração de funções auxiliares para renderizar texto dentro da janela
// OpenGL. Estas funções estão definidas no arquivo "textrendering.cpp".
void TextRendering_Init();
float TextRendering_LineHeight(GLFWwindow* window);
float TextRendering_CharWidth(GLFWwindow* window);
void TextRendering_PrintString(GLFWwindow* window, const std::string& str, float x, float y, float scale = 1.0f);
void TextRendering_PrintMatrix(GLFWwindow* window, glm::mat4 M, float x, float y, float scale = 1.0f);

// Funções abaixo renderizam como texto na janela OpenGL algumas matrizes e
// outras informações do programa. Definidas após main().
void TextRendering_ShowEulerAngles(GLFWwindow* window, GameObject*);
void TextRendering_ShowFramesPerSecond(GLFWwindow* window);

// Funções callback para comunicação com o sistema operacional e interação do
// usuário. Veja mais comentários nas definições das mesmas, abaixo.
void printGPUInfo();

#define FPS 60.0
#define SECONDS_PER_FRAME 1.0/FPS

int main() {
    Window window;
    Renderer renderer(window.width, window.height);

    printGPUInfo();

    FirstScene firstLevel;
    Game game(firstLevel, firstLevel.player);
    window.setGame(&game);

    std::vector<std::tuple<int, Command*>> commandLst = {
        std::make_tuple(GLFW_KEY_W,      new MoveCommand(game, MoveCommand::FORWARD)),
        std::make_tuple(GLFW_KEY_S,      new MoveCommand(game, MoveCommand::BACKWARD)),
        std::make_tuple(GLFW_KEY_A,      new MoveCommand(game, MoveCommand::LEFT)),
        std::make_tuple(GLFW_KEY_D,      new MoveCommand(game, MoveCommand::RIGHT)),
        std::make_tuple(GLFW_KEY_ESCAPE, new EscCommand(game)),
        std::make_tuple(GLFW_KEY_R,      new ReloadShadersCommand(renderer, game)),
        std::make_tuple(GLFW_MOUSE_BUTTON_LEFT, new PrimaryActionCommand(game)),
        std::make_tuple(GLFW_MOUSE_BUTTON_RIGHT, new SecondaryActionCommand(game)),
        std::make_tuple(GLFW_KEY_O,      new RotateTarget(game, RotateTarget::LEFT)),
        std::make_tuple(GLFW_KEY_P,      new RotateTarget(game, RotateTarget::RIGHT))
        //std::make_tuple(GLFW_MOUSE_BUTTON_MIDLE)
    };

    InputManager input(commandLst, game);
    window.setKeyCallbacks(&input);

    // Inicializamos o código para renderização de texto.
    TextRendering_Init();

    double lastFrameTime = glfwGetTime();

    while (!window.shouldClose()) {
        game.update();
        window.pollEvents();
        input.handleInput();
        double elapsedTime = glfwGetTime() - lastFrameTime;
        if (elapsedTime >= SECONDS_PER_FRAME) {
            renderer.draw(game);
            //TextRendering_ShowEulerAngles(window.window, game.cameraFollowing);
            TextRendering_ShowFramesPerSecond(window.window);
            window.swapBuffers();
            lastFrameTime = glfwGetTime();
        }
    }

    return 0;
}

void printGPUInfo() {
    // Imprimimos no terminal informações sobre a GPU do sistema
    const GLubyte* vendor = glGetString(GL_VENDOR);
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* glversion = glGetString(GL_VERSION);
    const GLubyte* glslversion = glGetString(GL_SHADING_LANGUAGE_VERSION);

    const char* glfwVersion = glfwGetVersionString();

    printf("GPU: %s, %s, OpenGL %s, GLSL %s\nGLFW Version: %s \n", vendor, renderer, glversion, glslversion, glfwVersion);
}

void TextRendering_ShowEulerAngles(GLFWwindow* window, GameObject* gameObject) {
    float pad = TextRendering_LineHeight(window);

    char buffer[120];
    snprintf(buffer, 120, "Angles Theta:(%.2f) Phi:(%.2f)\n", gameObject->theta, gameObject->phi);

    TextRendering_PrintString(window, buffer, -1.0f + pad / 10, -1.0f + 2 * pad / 10, 1.0f);
}

// Escrevemos na tela o número de quadros renderizados por segundo (frames per
// second).
void TextRendering_ShowFramesPerSecond(GLFWwindow* window) {
    // Variáveis estáticas (static) mantém seus valores entre chamadas
    // subsequentes da função!
    static float old_seconds = (float)glfwGetTime();
    static int ellapsed_frames = 0;
    static char buffer[20] = "?? fps";
    static int numchars = 7;

    ellapsed_frames += 1;

    // Recuperamos o número de segundos que passou desde a execução do programa
    float seconds = (float)glfwGetTime();

    // Número de segundos desde o último cálculo do fps
    float ellapsed_seconds = seconds - old_seconds;

    if (ellapsed_seconds > 1.0f) {
        numchars = snprintf(buffer, 20, "%.2f fps", ellapsed_frames / ellapsed_seconds);

        old_seconds = seconds;
        ellapsed_frames = 0;
    }

    float lineheight = TextRendering_LineHeight(window);
    float charwidth = TextRendering_CharWidth(window);

    TextRendering_PrintString(window, buffer, 1.0f - (numchars + 1) * charwidth, 1.0f - lineheight, 1.0f);
}
