#pragma once

#include "Renderer.h"

class Texture
{
public:
    Texture(const std::string& path);
    ~Texture();
    
    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

    int GetWidth() const { return Width_; }
    int GetHeight() const { return Height_; }
    
private:
    unsigned int RendererID_;
    std::string FilePath_;
    unsigned char* LocalBuffer_;
    int Width_, Height_, BPP_;
};
