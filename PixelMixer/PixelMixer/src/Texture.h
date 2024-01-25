#pragma once
#include <string>

class Texture
{
public:
    Texture(const std::string& path);
    Texture(unsigned int width, unsigned int height);
    ~Texture();
    
    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

    int GetWidth() const { return Width_; }
    int GetHeight() const { return Height_; }
    unsigned int GetRendererID() const { return RendererID_; }
    
private:
    unsigned int RendererID_;
    std::string FilePath_;
    unsigned char* LocalBuffer_;
    int Width_, Height_, BPP_;
};
