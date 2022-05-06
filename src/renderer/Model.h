#pragma once

#include <vector>
#include <string>
#include <tiny_obj_loader.h>
#include "BoundingBox.h"

class Model {
private:
    void computeNormals();
    void buildTriangles();
    void loadModel();
    void loadTexture();
    void createVBObject(const void* data, unsigned int size, int location, int dimensions);
    void createIBObject(const void* data, unsigned int size);
    void createDebugModel();
public:
    enum RenderProjectType { SPHERIC, PLANAR, FROM_MODEL, REPEAT };
    Model(std::string filename);
    Model(std::string filename, RenderProjectType renderType);
    std::string name;
    tinyobj::attrib_t                 attrib;
    std::vector<tinyobj::shape_t>     shapes;
    std::vector<tinyobj::material_t>  materials;
    std::vector<BoundingBox> boundingBoxes;
    std::vector<int> renderingMode;
    std::vector<int> firstIndex;
    std::vector<int> numIndexes;
    unsigned int vaoId;
    std::vector<std::string> shapeName;
    unsigned int textureId;
    RenderProjectType renderType;

    unsigned int vaoDebugId;
    int debugRenderingMode;
    int debugFirstIndex;
    int debugNumIndexes;
};