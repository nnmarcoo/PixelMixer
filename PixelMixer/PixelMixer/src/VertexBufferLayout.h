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

    VertexBufferElement(unsigned int t,unsigned int c,bool n):
        type(t), count(c),normalized(n)
    {

    }
};

class VertexBufferLayout {
public:
    VertexBufferLayout() : Stride_(0) {}

    template<typename T>
    void Push(unsigned int count );

    template<>
    void Push<float>(unsigned int count) {
        Elements_.push_back(VertexBufferElement({ GL_FLOAT,count,GL_FALSE }));
        Stride_ += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
    }

    template<>
    void Push<unsigned int>(unsigned int count) {
        Elements_.push_back(VertexBufferElement({ GL_UNSIGNED_INT,count,GL_FALSE }));
        Stride_ += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
    }

    template<>
    void Push<unsigned char>(unsigned int count) {
        Elements_.push_back(VertexBufferElement({ GL_UNSIGNED_BYTE,count,GL_TRUE }));
        Stride_ += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
    }

    inline const std::vector<VertexBufferElement> GetElements() const& { return Elements_;}
    unsigned int GetStride() const { return Stride_;}
    
private:
    std::vector<VertexBufferElement> Elements_;
    unsigned int Stride_;
};
