#pragma once

#include <iostream>
#include <tiny_obj_loader.h>


class Model {
private:
    void computeNormals();
public:
    Model();
    Model(std::string filename);
    std::string name;
    tinyobj::attrib_t                 attrib;
    std::vector<tinyobj::shape_t>     shapes;
    std::vector<tinyobj::material_t>  materials;
};