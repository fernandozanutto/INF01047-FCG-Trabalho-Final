#pragma once

#include <map>
#include <string>

#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include "Model.h"
#include "../game/BaseScene.h"
#include "../game/Game.h"

class Renderer {
public: 
    Renderer(unsigned int screenWidth, unsigned int screenHeight);
    ~Renderer();
    void draw(Game& game);
    void loadShaders();
private:
    float screenWidth;
    float screenHeight;
    GLint bbox_min_uniform;
    GLint bbox_max_uniform;

    float fov;

    unsigned int shader3dId;     //first shader, used for normal rendering, 
                                        //which is then bound to a texture

    unsigned int shader2dId;    //second shader, used for rendering from a texture
                                        //also used for ui elements

    unsigned int modelUniformId;
    unsigned int viewUniformId;
    unsigned int projectionUniformId;
    unsigned int object_id_uniform;
    unsigned int lightingUniformId;
    unsigned int handUniformId;
    unsigned int groundUniformId;
    unsigned int camPosUniformId;
    unsigned int camDirUniformId;
    unsigned int modelUniform2dId;

    void renderTextureToScreen();       //render texture input into the entire screen

    unsigned int LoadVertexShader(std::string name);   
    unsigned int LoadFragmentShader(std::string name);
    void LoadShader(const char* filename, unsigned int shader_id);
    unsigned int CreateGpuProgram(unsigned int vertexId, unsigned int fragmentId); 
    void drawObject(GameObject& object);
};