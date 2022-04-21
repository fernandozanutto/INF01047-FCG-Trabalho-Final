#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <tiny_obj_loader.h>
#include <stack>

class Model {
private:
    void computeNormals();
    void buildTrianglesAndAddToVirtualScene();
    void pushMatrix(glm::mat4 M);
    void popMatrix(glm::mat4& M);
    std::stack<glm::mat4> g_MatrixStack;
public:
    Model();
    Model(std::string filename);
    std::string name;
    tinyobj::attrib_t                 attrib;
    std::vector<tinyobj::shape_t>     shapes;
    std::vector<tinyobj::material_t>  materials;
    std::vector<glm::vec3> bbox_min;
    std::vector<glm::vec3> bbox_max;
    std::vector<int> renderingMode;
    std::vector<int> firstIndex;
    std::vector<int> numIndexes;
    std::vector<int> vaoId;
    std::vector<std::string> shapeName;
};