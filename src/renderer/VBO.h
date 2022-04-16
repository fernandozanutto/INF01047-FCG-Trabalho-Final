#pragma once 

/**
 * @brief Verter Buffer Object
 * 
 */
class VBO {
private:
    unsigned int bufferId;
public:
    VBO(const void* data, unsigned int size, int location, int dimensions);
};