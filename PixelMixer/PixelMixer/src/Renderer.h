#pragma once
#include <GL/glew.h>

#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"

#ifdef _WIN32
#define DEBUG_BREAK() __debugbreak()
#else
#include "signal.h"
#define DEBUG_BREAK() raise(SIGTRAP)
#endif
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();x;ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer {
public: // todo move elapsed time here
    static void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
    static void Draw(const Shader& shader);
    static void Clear();
};