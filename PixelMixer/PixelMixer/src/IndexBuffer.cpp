#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) : Count_(count) {
    ASSERT(sizeof(unsigned int) == sizeof(GLuint))
    
    GLCall(glGenBuffers(1, &RendererID_))
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RendererID_))
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW)) // can be diff data type // may change to GLuint
}

IndexBuffer::~IndexBuffer() {
    GLCall(glDeleteBuffers(1, &RendererID_))
}

void IndexBuffer::Bind() const {
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RendererID_))
}

void IndexBuffer::Unbind() const {
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0))
}
