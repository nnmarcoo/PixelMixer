#include "ViewportPanel.h"
#include <wx/image.h>

#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

//todo add checkerboard

//todo glDeleteProgram(shader somewhere

BEGIN_EVENT_TABLE(ViewportPanel, wxGLCanvas)
    EVT_PAINT(ViewportPanel::OnPaint)
    EVT_IDLE(ViewportPanel::OnIdle)
    EVT_SIZE(ViewportPanel::OnSize)
END_EVENT_TABLE()

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath) { // can be optimized
    std::ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
        
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else {
            ss[(int)type] << line << "\n";
        }
    }
    return { ss[0].str(), ss[1].str()};
}

static unsigned int CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str(); // returns pointer to beginning of data
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (!result) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    
    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDetachShader(program, vs);
    glDetachShader(program, fs);

    return program;
}

// new int[] {WX_GL_CORE_PROFILE, WX_GL_MAJOR_VERSION, 3, WX_GL_MINOR_VERSION, 3, 0} // doesn't work
ViewportPanel::ViewportPanel(wxWindow* parent) : wxGLCanvas(parent, wxID_ANY, nullptr, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE) {
    //wxInitAllImageHandlers();
    //image.LoadFile("C:/Users/marco/Desktop/ahdjahda.png", wxBITMAP_TYPE_ANY); // test
    
    context_ = new wxGLContext(this);
    SetCurrent(*context_);
    
    GLenum glewInitResult = glewInit();
    if (glewInitResult != GLEW_OK) {
        std::cerr << "GLEW initialization failed: " << glewGetErrorString(glewInitResult) << std::endl;
        return;
    }
    std::cout << glGetString(GL_VERSION) << "\n" << glGetString(GL_RENDERER) << "\n" << std::endl; // debug

    glClearColor(0.2109375f, 0.22265625f, 0.2421875f, 1.0);  // Set clear color to #36393e

    constexpr float positions[] = {
        -0.5f, -0.5f, // 0
         0.5f, -0.5f, // 1
         0.5f,  0.5f, // 2
        -0.5f,  0.5f, // 3
    };

    const unsigned int indices[] = { // can be char to save on mem
        0, 1, 2,
        2, 3, 0
    };
    
    vb_ = new VertexBuffer(positions, 4 * 2 * sizeof(float));
    va_ = new VertexArray();
    layout_ = new VertexBufferLayout();
    
    layout_->Push<float>(2);
    va_->AddBuffer(*vb_, *layout_);
    va_->Bind();

    ib_ = new IndexBuffer(indices, 6);
    ib_->Bind();
    
    ShaderProgramSource source = ParseShader("res/shaders/Test.shader");
    shader_ = CreateShader(source.VertexSource, source.FragmentSource);
    GLCall(glUseProgram(shader_))

    GLCall(location_ = glGetUniformLocation(shader_, "u_Color"))
    //ASSERT(location != -1)
    GLCall(glUniform4f(location_, 0.2f, 0.3f, 0.8f, 1.0f))
}

void ViewportPanel::render() {
    if (!IsShown()) return;
    //SetCurrent(*context); // unnecessary because there is only 1 context?
    glClear(GL_COLOR_BUFFER_BIT);
    
    GLCall(glUseProgram(shader_))
    GLCall(glUniform4f(location_, r_, 0.3f, 0.8f, 1.0f))
    
    GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr))

    if (r_ > 1.0f)
        increment_ = -0.05f;
    else if (r_ < 0.0f)
        increment_ = 0.05f;
    r_ += increment_;
    
    SwapBuffers();
}

void ViewportPanel::OnPaint(wxPaintEvent& e) {
    render();
}

void ViewportPanel::OnSize(wxSizeEvent& e) {
    if (initialized_) return;

    const wxSize viewport = GetSize();
    glViewport(0, 0, viewport.GetWidth(), viewport.GetHeight());
    if (viewport.GetWidth() > 10) initialized_ = true;
}

void ViewportPanel::OnIdle(wxIdleEvent& e) {
        Refresh();
}

ViewportPanel::~ViewportPanel() {
    delete ib_;
    delete vb_;
    delete va_;
    delete layout_;
    delete context_; // delete context last to avoid error loop
}
