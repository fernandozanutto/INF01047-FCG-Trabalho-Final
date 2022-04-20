#include "Renderer.h"

#include <string>
#include <sstream>
#include <fstream>
#include <map>
#include <iostream>

#include <glad/glad.h>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include "Matrices.h"
#include "Model.h"
#include "../game/GameObject.h"
#include "../game/BaseScene.h"
#include "SceneObject.h"


#define FOV 3.141592f/3.0f
#define NEARPLANE -0.1f
#define FARPLANE -400.0f

Renderer::Renderer(unsigned int screenWidth, unsigned int screenHeight) {
    unsigned int vertexShader3dId     = LoadVertexShader("shader_vertex.glsl");
    unsigned int fragmentShader3dId   = LoadFragmentShader("shader_fragment.glsl");
    unsigned int vertexShader2dId     = LoadVertexShader("shader_vertex.glsl");
    unsigned int fragmentShader2dId   = LoadFragmentShader("shader_fragment.glsl");

    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;

    this->fov = FOV;

    this->shader3dId = CreateGpuProgram(vertexShader3dId, fragmentShader3dId);
    this->shader2dId = CreateGpuProgram(vertexShader2dId, fragmentShader2dId);
    this->modelUniformId      = glGetUniformLocation(this->shader3dId, "model"); 
    this->viewUniformId       = glGetUniformLocation(this->shader3dId, "view"); 
    this->projectionUniformId = glGetUniformLocation(this->shader3dId, "projection"); 
    this->object_id_uniform = glGetUniformLocation(this->shader3dId, "object_id");    // Variável "object_id" em shader_fragment.glsl
    //this->lightingUniformId = glGetUniformLocation(this->shader3dId, "lightingIsEnabled"); 
    //this->handUniformId = glGetUniformLocation(this->shader3dId, "isRenderingHand"); 
    //this->groundUniformId = glGetUniformLocation(this->shader3dId, "isRenderingGround"); 
    //this->camPosUniformId = glGetUniformLocation(this->shader3dId, "cameraPosition"); 
    //this->camDirUniformId = glGetUniformLocation(this->shader3dId, "cameraDirection"); 
    this->modelUniform2dId      = glGetUniformLocation(this->shader2dId, "model"); 
    this->bbox_min_uniform = glGetUniformLocation(this->shader3dId, "bbox_min");
    this->bbox_max_uniform = glGetUniformLocation(this->shader3dId, "bbox_max");
}

Renderer::~Renderer() {
    glDeleteProgram(shader3dId);
    glDeleteProgram(shader2dId);
}

void Renderer::draw(glm::mat4 cameraView, BaseScene& scene) {
    glViewport(0,0, screenWidth, screenHeight);
    // Definimos a cor do "fundo" do framebuffer como branco.  Tal cor é
    // definida como coeficientes RGBA: Red, Green, Blue, Alpha; isto é:
    // Vermelho, Verde, Azul, Alpha (valor de transparência).
    // Conversaremos sobre sistemas de cores nas aulas de Modelos de Iluminação.
    //
    //           R     G     B     A
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // "Pintamos" todos os pixels do framebuffer com a cor definida acima,
    // e também resetamos todos os pixels do Z-buffer (depth buffer).
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST); 
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // Pedimos para a GPU utilizar o programa de GPU criado acima (contendo
    // os shaders de vértice e fragmentos).
    (glUseProgram(shader3dId));

    float screenRatio = (float)screenWidth / (float)screenHeight;

    glm::mat4 projection = Matrix_Perspective(fov, screenRatio, NEARPLANE, FARPLANE);

    (glUniformMatrix4fv(viewUniformId, 1 , GL_FALSE , glm::value_ptr(cameraView)));
    (glUniformMatrix4fv(projectionUniformId, 1 , GL_FALSE , glm::value_ptr(projection)));

    for (unsigned int i=0; i < scene.gameObjects.size(); i++) {
        GameObject object = scene.gameObjects[i];
        glUniformMatrix4fv(modelUniformId, 1, GL_FALSE, glm::value_ptr(object.getModelMatrix()));
        glUniform1i(object_id_uniform, i);
        drawObject(object.getModel());
    }
}

void Renderer::drawObject(Model* model) {
    if (model == NULL) {
        std::cout << "NULOOOOO" << std::endl;
    }
    
    int size = model->vaoId.size();
    for (int i=0; i < size; i++) {
        glBindVertexArray(model->vaoId[i]);
        // Setamos as variáveis "bbox_min" e "bbox_max" do fragment shader
        // com os parâmetros da axis-aligned bounding box (AABB) do modelo.
        glm::vec3 bbox_min = model->bbox_min[i];
        glm::vec3 bbox_max = model->bbox_max[i];
        glUniform4f(this->bbox_min_uniform, bbox_min.x, bbox_min.y, bbox_min.z, 1.0f);
        glUniform4f(this->bbox_max_uniform, bbox_max.x, bbox_max.y, bbox_max.z, 1.0f);

        
        glDrawElements(
            model->renderingMode[i],
            model->numIndexes[i],
            GL_UNSIGNED_INT,
            (void*)(model->firstIndex[i] * sizeof(GLuint)));

        glBindVertexArray(0);
    }
}


void Renderer::renderTextureToScreen() {
    (glUseProgram(this->shader2dId));
    glm::mat4 model = Matrix_Scale(3.0/4.0, 1.0, 1.0);
    (glUniformMatrix4fv(this->modelUniform2dId, 1, GL_FALSE, glm::value_ptr(model)));
    (glBindFramebuffer(GL_FRAMEBUFFER, 0));
    (glDisable(GL_DEPTH_TEST));
    (glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    (glClear(GL_COLOR_BUFFER_BIT));
    //drawModel(*screenQuad);
}


unsigned int Renderer::LoadVertexShader(std::string name) {
    std::string filename = "./shaders/" + name;
    GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);

    LoadShader(filename.c_str(), vertex_shader_id);

    return vertex_shader_id;
}

unsigned int Renderer::LoadFragmentShader(std::string name) {
    std::string filename = "./shaders/" + name;
    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    LoadShader(filename.c_str(), fragmentShaderId);

    return fragmentShaderId;
}

void Renderer::LoadShader(const char* filename, unsigned int shader_id) {
    std::ifstream file;
    try {
        file.exceptions(std::ifstream::failbit);
        file.open(filename);
    } catch ( std::exception& e ) {
        fprintf(stderr, "ERROR: Cannot open file \"%s\".\n", filename);
        std::exit(EXIT_FAILURE);
    }
    std::stringstream shader;
    shader << file.rdbuf();
    std::string str = shader.str();
    const GLchar* shader_string = str.c_str();
    const GLint   shader_string_length = static_cast<GLint>( str.length() );

    (glShaderSource(shader_id, 1, &shader_string, &shader_string_length));

    (glCompileShader(shader_id));

    GLint compiled_ok;
    (glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled_ok));

    GLint log_length = 0;
    (glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length));

    GLchar* log = new GLchar[log_length];
    (glGetShaderInfoLog(shader_id, log_length, &log_length, log));

    if ( log_length != 0 ) {
        std::string  output;

        if ( !compiled_ok ) {
            output += "ERROR: OpenGL compilation of \"";
            output += filename;
            output += "\" failed.\n";
            output += "== Start of compilation log\n";
            output += log;
            output += "== End of compilation log\n";
        } else {
            output += "WARNING: OpenGL compilation of \"";
            output += filename;
            output += "\".\n";
            output += "== Start of compilation log\n";
            output += log;
            output += "== End of compilation log\n";
        }

        fprintf(stderr, "%s", output.c_str());
    }

    delete [] log;
}

unsigned int Renderer::CreateGpuProgram(unsigned int vertexId, unsigned int fragmentId) {
    GLuint program_id = glCreateProgram();

    glAttachShader(program_id, vertexId);
    glAttachShader(program_id, fragmentId);

    glLinkProgram(program_id);

    GLint linked_ok = GL_FALSE;
    glGetProgramiv(program_id, GL_LINK_STATUS, &linked_ok);

    if ( linked_ok == GL_FALSE ) {
        GLint log_length = 0;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);

        GLchar* log = new GLchar[log_length];

        glGetProgramInfoLog(program_id, log_length, &log_length, log);

        std::string output;

        output += "ERROR: OpenGL linking of program failed.\n";
        output += "== Start of link log\n";
        output += log;
        output += "\n== End of link log\n";

        delete [] log;

        fprintf(stderr, "%s", output.c_str());
    }

    // Os "Shader Objects" podem ser marcados para deleção após serem linkados
    glDeleteShader(vertexId);
    glDeleteShader(fragmentId);

    return program_id;
}