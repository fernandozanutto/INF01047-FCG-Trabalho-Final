#version 330 core

// Atributos de fragmentos recebidos como entrada ("in") pelo Fragment Shader.
// Neste exemplo, este atributo foi gerado pelo rasterizador como a
// interpolação da posição global e a normal de cada vértice, definidas em
// "shader_vertex.glsl" e "main.cpp".
in vec4 position_world;
in vec4 normal;

// Posição do vértice atual no sistema de coordenadas local do modelo.
in vec4 position_model;

// Coordenadas de textura obtidas do arquivo OBJ (se existirem!)
in vec2 texcoords;

// Matrizes computadas no código C++ e enviadas para a GPU
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Identificador que define qual objeto está sendo desenhado no momento
#define SPHERIC 0
#define PLANAR  1
#define FROM_MODEL  2
#define REPEAT  3
uniform int object_id;

in vec4 axis_color;
uniform bool is_drawing_axis;

// Parâmetros da axis-aligned bounding box (AABB) do modelo
uniform vec4 bbox_min;
uniform vec4 bbox_max;

// Variáveis para acesso das imagens de textura
uniform sampler2D textureImage;

// O valor de saída ("out") de um Fragment Shader é a cor final do fragmento.
out vec4 color;

// Constantes
#define M_PI   3.14159265358979323846
#define M_PI_2 1.57079632679489661923

void main() {

    if (is_drawing_axis) {
        color = axis_color;
    } else {
        // Obtemos a posição da câmera utilizando a inversa da matriz que define o
        // sistema de coordenadas da câmera.
        vec4 origin = vec4(0.0, 0.0, 0.0, 1.0);
        vec4 camera_position = inverse(view) * origin;

        // O fragmento atual é coberto por um ponto que percente à superfície de um
        // dos objetos virtuais da cena. Este ponto, p, possui uma posição no
        // sistema de coordenadas global (World coordinates). Esta posição é obtida
        // através da interpolação, feita pelo rasterizador, da posição de cada
        // vértice.
        vec4 p = position_world;

        // Normal do fragmento atual, interpolada pelo rasterizador a partir das
        // normais de cada vértice.
        vec4 n = normalize(normal);

        // Vetor que define o sentido da fonte de luz em relação ao ponto atual.
        vec4 l = normalize(vec4(1.0,1.0,0.0,0.0));

        // Vetor que define o sentido da câmera em relação ao ponto atual.
        vec4 v = normalize(camera_position - p);

        // Coordenadas de textura U e V
        float U = 0.0;
        float V = 0.0;

        if (object_id == SPHERIC) {
            // projeção esférica
            vec4 bbox_center = (bbox_min + bbox_max) / 2.0;
            vec4 c = bbox_center;

            vec4 pp = c + normalize(position_model - c);
            pp = pp - c;

            float tetha = atan(pp.x, pp.z);
            float phi = asin(pp.y);

            U = (tetha + M_PI) / (2*M_PI);
            V = (phi + (M_PI/2)) / M_PI;
        } else if ( object_id == PLANAR) {
            // projeção planar
            float minx = bbox_min.x;
            float maxx = bbox_max.x;

            float miny = bbox_min.y;
            float maxy = bbox_max.y;

            float minz = bbox_min.z;
            float maxz = bbox_max.z;

            float px = (position_model.x - minx) / (maxx - minx);
            float py = (position_model.y - miny) / (maxy - miny);

            U = px;
            V = py;
        } else if (object_id == FROM_MODEL) {
            // Coordenadas de textura do plano, obtidas do arquivo OBJ.
            U = texcoords.x;
            V = texcoords.y;
        } else if (object_id == REPEAT) {
            float x = position_model.x * 10;
            float y = position_model.z * 10;
            
            while (y > 1.0) y--;
            while (x > 1.0) x--;
            while (y < 0) y++;
            while (x < 0) x++;
            U = x;
            V = y;
        } else {
            U = 0;
            V = 0;
        }

        // Obtemos a refletância difusa a partir da leitura da imagem textureImage
        vec3 Kd0 = texture(textureImage, vec2(U,V)).rgb;

        // Equação de Iluminação
        float lambert = max(0,dot(n,l));
        float lambert2 = max(0,dot(-n,l));

        color.rgb = Kd0 * (lambert + 0.01);

        // NOTE: Se você quiser fazer o rendering de objetos transparentes, é
        // necessário:
        // 1) Habilitar a operação de "blending" de OpenGL logo antes de realizar o
        //    desenho dos objetos transparentes, com os comandos abaixo no código C++:
        //      glEnable(GL_BLEND);
        //      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // 2) Realizar o desenho de todos objetos transparentes *após* ter desenhado
        //    todos os objetos opacos; e
        // 3) Realizar o desenho de objetos transparentes ordenados de acordo com
        //    suas distâncias para a câmera (desenhando primeiro objetos
        //    transparentes que estão mais longe da câmera).
        // Alpha default = 1 = 100% opaco = 0% transparente
        color.a = 1;

        // Cor final com correção gamma, considerando monitor sRGB.
        // Veja https://en.wikipedia.org/w/index.php?title=Gamma_correction&oldid=751281772#Windows.2C_Mac.2C_sRGB_and_TV.2Fvideo_standard_gammas
        color.rgb = pow(color.rgb, vec3(1.0,1.0,1.0)/2.2);
    }
}

