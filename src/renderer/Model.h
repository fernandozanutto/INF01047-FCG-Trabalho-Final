#pragma once

#include <tiny_obj_loader.h>


class Model {
public:
    Model(const char* filename);
    tinyobj::attrib_t                 attrib;
    std::vector<tinyobj::shape_t>     shapes;
    std::vector<tinyobj::material_t>  materials;
};