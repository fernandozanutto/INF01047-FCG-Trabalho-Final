#pragma once 

/**
 * @brief Index Buffer Object
 * 
 */
class IBO {
private:
    unsigned int indexId;
public:
    IBO(const void* data, unsigned int size);
};