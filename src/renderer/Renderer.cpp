#include "Renderer.h"

#include <string>
#include <sstream>
#include <fstream>
#include <map>

#include <glad/glad.h>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include "Matrices.h"
#include "Model.h"
#include "../game/GameObject.h"
#include "SceneObject.h"

#define DOWNSCALE_FACTOR 1/4
//factor used in the game's pixelated effect

#define FOV 3.141592f/3.0f
#define NEARPLANE -0.1f
#define FARPLANE -400.0f

Renderer::Renderer(unsigned int screenWidth, unsigned int screenHeight, std::map<std::string, SceneObject> g_VirtualScene)
: g_VirtualScene(g_VirtualScene) {
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
    this->lightingUniformId = glGetUniformLocation(this->shader3dId, "lightingIsEnabled"); 
    this->handUniformId = glGetUniformLocation(this->shader3dId, "isRenderingHand"); 
    this->groundUniformId = glGetUniformLocation(this->shader3dId, "isRenderingGround"); 
    this->camPosUniformId = glGetUniformLocation(this->shader3dId, "cameraPosition"); 
    this->camDirUniformId = glGetUniformLocation(this->shader3dId, "cameraDirection"); 
    this->modelUniform2dId      = glGetUniformLocation(this->shader2dId, "model"); 
    this->bbox_min_uniform = glGetUniformLocation(this->shader3dId, "bbox_min");
    this->bbox_max_uniform = glGetUniformLocation(this->shader3dId, "bbox_max");
}

Renderer::~Renderer() {
    glDeleteProgram(shader3dId);
    glDeleteProgram(shader2dId);
}

void Renderer::draw(glm::mat4 cameraView, GLint object_id_uniform) {
    glViewport(0,0, screenWidth, screenHeight);
    //(glBindFramebuffer(GL_FRAMEBUFFER, downscaledBuffer.getId()));
    //uses downscaled screensize and binds framebuffer

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
    glm::vec4 camera_up_vector = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);                  // Vetor "up" fixado para apontar para o "céu" (eito Y global)

    //glm::mat4 view = Matrix_Camera_View(scene.player.getPosition(), scene.player.getFacing(), camera_up_vector);
    (glUniformMatrix4fv(viewUniformId, 1 , GL_FALSE , glm::value_ptr(cameraView)));
    (glUniformMatrix4fv(projectionUniformId, 1 , GL_FALSE , glm::value_ptr(projection)));
    
    #define SPHERE 0
    #define BUNNY 1
    #define PLANE 2
    #define SMALL_BUNNY 3

    // Desenhamos o modelo de um coelho pequeno
    GameObject coelhoPequeno;
    coelhoPequeno.scale(0.3f, 0.3f, 0.3f)->translate(0.0f, 2.0f, 0.0f);
    glUniformMatrix4fv(modelUniformId, 1, GL_FALSE, glm::value_ptr(coelhoPequeno.modelMatrix));
    glUniform1i(object_id_uniform, 10);
    DrawVirtualObject("bunny");

    // Desenhamos o modelo da esfera
    GameObject sphere;
    sphere.rotateY(glfwGetTime() * 0.1f)->rotateX(0.2f)->rotateZ(0.6f)->translate(-1.0f, 0, 0);
    glUniformMatrix4fv(modelUniformId, 1, GL_FALSE, glm::value_ptr(sphere.modelMatrix));
    glUniform1i(object_id_uniform, SPHERE);
    DrawVirtualObject("sphere");

    // Desenhamos o modelo do coelho
    GameObject coelho;
    coelho.rotateX(glfwGetTime() * 0.1f)->translate(1,0,0);
    glUniformMatrix4fv(modelUniformId, 1, GL_FALSE, glm::value_ptr(coelho.modelMatrix));
    glUniform1i(object_id_uniform, BUNNY);
    DrawVirtualObject("bunny");

    // Desenhamos o plano do chão
    GameObject floor;
    floor.scale(2.0f, 1.0f, 2.0f)->translate(0, -1.1f, 0);
    glUniformMatrix4fv(modelUniformId, 1, GL_FALSE, glm::value_ptr(floor.modelMatrix));
    glUniform1i(object_id_uniform, PLANE);
    DrawVirtualObject("plane");
}

void Renderer::DrawVirtualObject(const char* object_name) {
    // "Ligamos" o VAO. Informamos que queremos utilizar os atributos de
    // vértices apontados pelo VAO criado pela função BuildTrianglesAndAddToVirtualScene(). Veja
    // comentários detalhados dentro da definição de BuildTrianglesAndAddToVirtualScene().
    glBindVertexArray(this->g_VirtualScene[object_name].vertex_array_object_id);

    // Setamos as variáveis "bbox_min" e "bbox_max" do fragment shader
    // com os parâmetros da axis-aligned bounding box (AABB) do modelo.
    glm::vec3 bbox_min = g_VirtualScene[object_name].bbox_min;
    glm::vec3 bbox_max = g_VirtualScene[object_name].bbox_max;
    glUniform4f(this->bbox_min_uniform, bbox_min.x, bbox_min.y, bbox_min.z, 1.0f);
    glUniform4f(this->bbox_max_uniform, bbox_max.x, bbox_max.y, bbox_max.z, 1.0f);

    
    glDrawElements(
        this->g_VirtualScene[object_name].rendering_mode,
        this->g_VirtualScene[object_name].num_indices,
        GL_UNSIGNED_INT,
        (void*)(this->g_VirtualScene[object_name].first_index * sizeof(GLuint)));

    glBindVertexArray(0);
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