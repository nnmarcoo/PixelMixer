#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, const unsigned int size) {
    GLCall(glGenBuffers(1, &RendererID_))
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, RendererID_))
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW))
}

void VertexBuffer::Bind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, RendererID_))
}

void VertexBuffer::Unbind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0))
}

VertexBuffer::~VertexBuffer() {
    GLCall(glDeleteBuffers(1, &RendererID_))
}
