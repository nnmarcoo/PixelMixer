#pragma once
#include <stdexcept>
#include <vector>

#include "Renderer.h"

struct VertexBufferElement {
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int GetSizeOfType(unsigned int type) {
        switch(type) {
            case GL_FLOAT: return 4;
            case GL_UNSIGNED_BYTE: return 1;
            case GL_UNSIGNED_INT: return 4;
        default: return 0;
        }
    }
};

class VertexBufferLayout {
public:
    VertexBufferLayout() : Stride_(0) {}

    template<typename T>
    void Push(unsigned int count );

    std::vector<VertexBufferElement> GetElements() const & { return Elements_;}
    unsigned int GetStride() const { return Stride_;}
    
private:
    std::vector<VertexBufferElement> Elements_;
    unsigned int Stride_;
};


template<>
inline void VertexBufferLayout::Push<float>(unsigned int count) {
    Elements_.push_back({ GL_FLOAT,count,GL_FALSE });
    Stride_ += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
}

template<>
inline void VertexBufferLayout::Push<unsigned int>(unsigned int count) {
    Elements_.push_back({ GL_UNSIGNED_INT,count,GL_FALSE });
    Stride_ += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
}

template<>
inline void VertexBufferLayout::Push<unsigned char>(unsigned int count) {
    Elements_.push_back({ GL_UNSIGNED_BYTE,count,GL_TRUE });
    Stride_ += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
}