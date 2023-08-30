#include "ViewportPanel.h"
#include <wx/image.h>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

//todo add checkerboard

//todo glDeleteProgram(shader somewhere

BEGIN_EVENT_TABLE(ViewportPanel, wxGLCanvas)
    EVT_PAINT(ViewportPanel::OnPaint)
    EVT_IDLE(ViewportPanel::OnIdle)
    EVT_SIZE(ViewportPanel::OnSize)
END_EVENT_TABLE()

// new int[] {WX_GL_CORE_PROFILE, WX_GL_MAJOR_VERSION, 3, WX_GL_MINOR_VERSION, 3, 0} // doesn't work
ViewportPanel::ViewportPanel(wxWindow* parent, bool* DragState) : wxGLCanvas(parent, wxID_ANY, nullptr, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE), dragstate_(DragState) {
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

    shader_ = new Shader("res/shaders/Test.shader");
    shader_->Bind();
    shader_->SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);
}

void ViewportPanel::render() {
    if (!IsShown()) return;
    //SetCurrent(*context); // unnecessary because there is only 1 context?
    renderer_->Clear();
    
    shader_->SetUniform4f("u_Color", r_, 0.3f, 0.8f, 1.0f);
    renderer_->Draw(*va_, *ib_, *shader_);
    
    if (r_ > 1.0f)
        increment_ = -0.05f;
    else if (r_ < 0.0f)
        increment_ = 0.05f;
    r_ += increment_;
    
    SwapBuffers();
}

void ViewportPanel::OnPaint(wxPaintEvent& e) {
    if (*dragstate_) return;
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
    delete shader_;
    delete renderer_;
    delete context_; // delete context last to avoid error loop
}
