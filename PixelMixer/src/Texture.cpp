#include "Texture.h"
#include "Renderer.h"
#define STB_IMAGE_IMPLEMENTATION
#include "vendor/stb_image/stb_image.h"

Texture::Texture(const std::string& path) : RendererID_(0), FilePath_(path), LocalBuffer_(nullptr), Width_(0), Height_(0), BPP_(0){

    stbi_set_flip_vertically_on_load(1);
    LocalBuffer_ = stbi_load(path.c_str(), &Width_, &Height_, &BPP_, 4);
    
    GLCall(glGenTextures(1, &RendererID_))
    GLCall(glBindTexture(GL_TEXTURE_2D, RendererID_))

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST)) // GL_LINEAR
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST))
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE))
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE))

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Width_, Height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, LocalBuffer_))
    GLCall(glBindTexture(GL_TEXTURE_2D, 0))

    if (LocalBuffer_)
        stbi_image_free(LocalBuffer_);
}

Texture::Texture(unsigned width, unsigned height) : RendererID_(0), LocalBuffer_(nullptr), Width_(width), Height_(height), BPP_(0) {
    GLCall(glGenTextures(1, &RendererID_))
    GLCall(glBindTexture(GL_TEXTURE_2D, RendererID_))

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST)) // GL_LINEAR
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST))
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE))
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE))

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Width_, Height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, LocalBuffer_))
    GLCall(glBindTexture(GL_TEXTURE_2D, 0))
}

void Texture::Bind(unsigned slot) const {
    GLCall(glActiveTexture(GL_TEXTURE0 + slot))
    GLCall(glBindTexture(GL_TEXTURE_2D, RendererID_))
}

void Texture::Unbind() const {
    GLCall(glBindTexture(GL_TEXTURE_2D, 0))
}

Texture::~Texture() {
    GLCall(glDeleteTextures(1, &RendererID_))
}