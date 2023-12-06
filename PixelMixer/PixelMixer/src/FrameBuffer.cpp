#include "FrameBuffer.h"
#include <iostream>
#include "Renderer.h"
#include "Texture.h"

FrameBuffer::FrameBuffer(unsigned width, unsigned height) : RendererID_(0), TextureID_(0), RenderBufferID_(0), Width_(width), Height_(height), ClearColor_(1) {
    const Texture tempTexture(width, height);
    TextureID_ = tempTexture.GetRendererID();
    Width_ = tempTexture.GetWidth();
    Height_ = tempTexture.GetHeight();

    GLCall(glGenFramebuffers(1, &RendererID_))
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, RendererID_))
    
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TextureID_, 0))
    
    GLCall(glGenRenderbuffers(1, &RenderBufferID_))
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, RenderBufferID_))
    GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Width_, Height_))
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0))
    GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RenderBufferID_))
    
    const int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "Failure to create framebuffer." << std::endl;

    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0))
}

FrameBuffer::~FrameBuffer() {
    GLCall(glDeleteFramebuffers(1, &RendererID_))
    RendererID_ = 0;
    TextureID_ = 0;
    RenderBufferID_ = 0;
}

void FrameBuffer::Bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, RendererID_);
}

void FrameBuffer::Unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
