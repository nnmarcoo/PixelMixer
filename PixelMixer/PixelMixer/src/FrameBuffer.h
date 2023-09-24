#pragma once
#include "vendor/glm/glm.hpp"

class FrameBuffer
{
public:
    FrameBuffer(unsigned int width, unsigned int height);
    ~FrameBuffer();

    unsigned int GetFBO() const { return RendererID_; }
    unsigned int GetTexture() const { return TextureID_; }
    unsigned int GetRBO() const { return RenderBufferID_; }
    void GetSize(unsigned int& w, unsigned int& h) const { w = Width_; h = Height_; }
    void SetClearColor(float r, float g, float b, float a) { ClearColor_ = glm::vec4(r,g,b,a); }
    void GetClearColor(float& r, float& g, float& b, float& a) const { r = ClearColor_.x; g = ClearColor_.y; b = ClearColor_.z; a = ClearColor_.w; }
    void setHeight(unsigned int h);
    void setWidth(unsigned int w);

    void Bind();
    void Unbind();

private:
    unsigned int RendererID_;
    unsigned int TextureID_;
    unsigned int RenderBufferID_;
    unsigned int Width_, Height_;
    glm::vec4 ClearColor_;

    void InitializeFrameBuffer();
    void DestroyFrameBuffer();
    void ResizeFrameBuffer();
    
};
