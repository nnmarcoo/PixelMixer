#include "ViewportPanel.h"

#include <wx/display.h>
#include <wx/image.h>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

//todo add checkerboard
//todo is it bad to reinitialize proj?

BEGIN_EVENT_TABLE(ViewportPanel, wxGLCanvas)
    EVT_PAINT(ViewportPanel::OnPaint)
    //EVT_IDLE(ViewportPanel::OnIdle)
    EVT_SIZE(ViewportPanel::OnSize)
END_EVENT_TABLE()

// new int[] {WX_GL_CORE_PROFILE, WX_GL_MAJOR_VERSION, 3, WX_GL_MINOR_VERSION, 3, 0} // doesn't work
ViewportPanel::ViewportPanel(wxWindow* parent, bool* DragState) : wxGLCanvas(parent, wxID_ANY, nullptr, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE), dragstate_(DragState) {
    context_ = new wxGLContext(this);
    SetCurrent(*context_);

    const GLenum glewInitResult = glewInit();
    if (glewInitResult != GLEW_OK) {
        std::cerr << "GLEW initialization failed: " << glewGetErrorString(glewInitResult) << std::endl;
        return;
    }
    std::cout << glGetString(GL_VERSION) << "\n" << glGetString(GL_RENDERER) << "\n" << std::endl; // debug

    glClearColor(0.2109375f, 0.22265625f, 0.2421875f, 1.0);  // Set clear color to #36393e
    
    const wxSize displaysize = wxDisplay(wxDisplay::GetFromPoint(wxGetMousePosition())).GetGeometry().GetSize();

    const float s = 500; // todo make relative to screen size instead of constant
    constexpr float positions[] = {
         -1.0f*s, -1.0f*s, 0.0f, 0.0f, // 0 bottom-left
          1.0f*s, -1.0f*s, 1.0f, 0.0f, // 1 bottom-right
          1.0f*s,  1.0f*s, 1.0f, 1.0f, // 2 top-right
         -1.0f*s,  1.0f*s, 0.0f, 1.0f  // 3 top-left
    };

    const unsigned int indices[] = { // can be char to save on mem
        0, 1, 2,
        2, 3, 0
    };

    GLCall(glEnable(GL_BLEND))
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA))
    
    vb_ = new VertexBuffer(positions, 4 * 4 * sizeof(float)); // points * components * how big each component is
    va_ = new VertexArray();
    layout_ = new VertexBufferLayout();
    
    layout_->Push<float>(2);
    layout_->Push<float>(2);
    va_->AddBuffer(*vb_, *layout_);
    va_->Bind();

    ib_ = new IndexBuffer(indices, 6);
    ib_->Bind();

    const wxSize viewport = GetSize();
    const glm::mat4 proj = glm::ortho(-static_cast<float>(viewport.GetWidth()), static_cast<float>(viewport.GetWidth()), -static_cast<float>(viewport.GetHeight()), static_cast<float>(viewport.GetHeight()), -1.0f, 1.0f);
    
    shader_ = new Shader("res/shaders/Test.shader");
    shader_->Bind();
    shader_->SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);
    shader_->SetUniformMat4f("u_MVP", proj);
    
    texture_ = new Texture("res/textures/test.png");
    texture_->Bind();
    shader_->SetUniform1i("u_Texture", 0);
}

void ViewportPanel::render() {
    if (!IsShown()) return;
    //SetCurrent(*context); // unnecessary because there is only 1 context?
    renderer_->Clear();

    const wxSize viewport = GetSize();
    const glm::mat4 proj = glm::ortho(-static_cast<float>(viewport.GetWidth()), static_cast<float>(viewport.GetWidth()), -static_cast<float>(viewport.GetHeight()), static_cast<float>(viewport.GetHeight()), -1.0f, 1.0f);
    shader_->SetUniformMat4f("u_MVP", proj);

    
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
    const wxSize viewport = GetSize();
    glViewport(0, 0, viewport.GetWidth(), viewport.GetHeight());
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
    delete texture_;
    delete context_; // delete context last to avoid error loop
}
