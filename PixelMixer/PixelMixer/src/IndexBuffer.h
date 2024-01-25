#pragma once

// Holds indices that point to specific vertices in a vertex buffer 
class IndexBuffer { // ibo
public:
    IndexBuffer(const unsigned int* data, unsigned int count); // can use other data types
    IndexBuffer() = default;
    ~IndexBuffer();
    void Bind() const;
    void Unbind() const;
    unsigned int GetCount() const { return Count_; }

private:
    unsigned int RendererID_;
    unsigned int Count_;
};
