#include "FrameBuffer.h"
#include <iostream>
#include "Renderer.h"

FrameBuffer::FrameBuffer(unsigned width, unsigned height) : RendererID_(0), TextureID_(0), RenderBufferID_(0), Width_(width), Height_(height), ClearColor_(1) {

    GLCall(glGenFramebuffers(1, &RendererID_))
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, RendererID_))

    // Create color texture
    GLCall(glGenTextures(1, &TextureID_))
    GLCall(glBindTexture(GL_TEXTURE_2D, TextureID_))
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width_, Height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr))
    GLCall(glBindTexture(GL_TEXTURE_2D, 0))
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TextureID_, 0))

    // Create depth/stencil renderbuffer
    GLCall(glGenRenderbuffers(1, &RenderBufferID_))
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, RenderBufferID_))
    GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Width_, Height_))
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0))
    GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RenderBufferID_))

    // Check for completeness
    const int status = GLCall(glCheckFramebufferStatus(GL_FRAMEBUFFER))
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
