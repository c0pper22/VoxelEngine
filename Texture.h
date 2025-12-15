#pragma once
#include <glad/glad.h>
#include <string>

class Texture {
public:
    unsigned int ID;
    int width, height, nrChannels;

    Texture(const char* path);
    ~Texture();

    void bind(unsigned int slot = 0) const;
    void unbind() const;
};