// Headers da biblioteca para carregar modelos obj
#include <tiny_obj_loader.h>

#include "Model.h"

Model::Model(const char* filename) {
    printf("Carregando modelo \"%s\"... ", filename);

    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename, NULL, true);

    if (!err.empty()) {
        fprintf(stderr, "\n%s\n", err.c_str());
    }

    if (!ret) {
        throw std::runtime_error("Erro ao carregar modelo.");
    }

    printf("OK.\n");
};
