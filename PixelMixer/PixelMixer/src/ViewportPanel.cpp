#include "ViewportPanel.h"

#include <wx/display.h>
//#include <wx/image.h>
#include <thread>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "vendor/glm/gtx/string_cast.hpp"
#include "vendor/glm/glm.hpp"

//todo add checkerboard
//todo fix zoomed pan speed
//todo fix zoom border restrictions
//todo waiting for media and animation when no media

BEGIN_EVENT_TABLE(ViewportPanel, wxGLCanvas)
    EVT_PAINT(ViewportPanel::OnPaint)
    //EVT_IDLE(ViewportPanel::OnIdle)
    EVT_SIZE(ViewportPanel::OnSize)

    EVT_RIGHT_DOWN(ViewportPanel::OnRightDown)
    EVT_RIGHT_UP(ViewportPanel::OnRightUp)
    EVT_LEFT_DCLICK(ViewportPanel::OnDoubleLeftClick)
    EVT_MOTION(ViewportPanel::OnMouseMove)
    EVT_MOUSEWHEEL(ViewportPanel::OnMouseWheel)
END_EVENT_TABLE()

// new int[] {WX_GL_CORE_PROFILE, WX_GL_MAJOR_VERSION, 3, WX_GL_MINOR_VERSION, 3, 0} // doesn't work
ViewportPanel::ViewportPanel(wxWindow* parent, bool* DragState) : wxGLCanvas(parent, wxID_ANY, nullptr, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE), wdragstate_(DragState) {
    context_ = new wxGLContext(this);
    SetCurrent(*context_);

    const GLenum glewInitResult = glewInit();
    if (glewInitResult != GLEW_OK) {
        std::cerr << "GLEW initialization failed: " << glewGetErrorString(glewInitResult) << std::endl;
        return;
    }
    std::cout << glGetString(GL_VERSION) << "\n" << glGetString(GL_RENDERER) << "\n" << std::endl; // debug

    GLCall(glEnable(GL_BLEND))                                      // Enable blending
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA))       // Blend the alpha channel
    GLCall(glClearColor(0.2109375f, 0.22265625f, 0.2421875f, 1.0))  // Set clear color to #36393e

    zoomfactor_ = 1.0f;
    base_ = glm::mat4(1.0f);
    view_ = scale(base_, glm::vec3(1, 1, 0));
    
    /* REST OF CONSTRUCTOR IS FOR TESTING */

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
    
    vb_ = new VertexBuffer(positions, 4 * 4 * sizeof(float)); // points * components * how big each component is
    va_ = new VertexArray();
    layout_ = new VertexBufferLayout();
    
    layout_->Push<float>(2);
    layout_->Push<float>(2);
    va_->AddBuffer(*vb_, *layout_);
    va_->Bind();

    ib_ = new IndexBuffer(indices, 6);
    ib_->Bind();
    
    shader_ = new Shader("res/shaders/Test.glsl");
    shader_->Bind();
    
    texture_ = new Texture("res/textures/debug.jpg");
    texture_->Bind();
    shader_->SetUniform1i("u_Texture", 0);
}

void ViewportPanel::render() {
    if (!IsShown()) return;
    //SetCurrent(*context); // unnecessary because there is only 1 context?
    renderer_->Clear();
    
    shader_->SetUniformMat4f("u_MVP", mvp_);
    
    renderer_->Draw(*va_, *ib_, *shader_);
    
    SwapBuffers();
}

void ViewportPanel::UpdateMVP() {
    modl_ = translate(base_, glm::vec3(loc_.x*static_cast<float>(viewport_.x)* 2 * (1 / zoomfactor_), -loc_.y*static_cast<float>(viewport_.y)* 2 * (1 / zoomfactor_), 0));
    mvp_ = proj_ * view_ * modl_;
}

void ViewportPanel::OnPaint(wxPaintEvent& e) {
    if (*wdragstate_) return;
    render();
}

void ViewportPanel::OnSize(wxSizeEvent& e) {
    viewport_ = GetSize();
    glViewport(0, 0, viewport_.x, viewport_.y);
    proj_ = glm::ortho(-static_cast<float>(viewport_.x), static_cast<float>(viewport_.x), -static_cast<float>(viewport_.y), static_cast<float>(viewport_.y), -1.0f, 1.0f);
    UpdateMVP();
}

void ViewportPanel::OnIdle(wxIdleEvent& e) {
    Refresh();
}

void ViewportPanel::OnRightDown(wxMouseEvent& e) { // todo instead of changing the mouse icon, should I make the mouse invisible, and set it back to where it was after the drag?
    SetCursor(wxCursor(wxCURSOR_SIZING));
    isDragging_ = true;
    dragStart_ = e.GetPosition();
    CaptureMouse();
}

void ViewportPanel::OnRightUp(wxMouseEvent& e) {
    SetCursor(wxCursor(wxCURSOR_DEFAULT));
    isDragging_ = false;
    if (HasCapture())
        ReleaseMouse();
    prevpos_ = loc_;
}

void ViewportPanel::OnDoubleLeftClick(wxMouseEvent& e) {
    CenterMedia();
}

void ViewportPanel::OnMouseMove(wxMouseEvent& e) {
    if (!isDragging_) return;

    std::thread calc([this, e] { //                 does this make a difference
        const wxPoint delta = e.GetPosition() - dragStart_;
        const float ratiox = static_cast<float>(delta.x) / static_cast<float>(viewport_.x);
        const float ratioy = static_cast<float>(delta.y) / static_cast<float>(viewport_.y);

        float newposx = ratiox + prevpos_.x;
        float newposy = ratioy + prevpos_.y;
    
        if (newposx*2*static_cast<float>(viewport_.x) > static_cast<float>(viewport_.x)) newposx = 0.5; // if image pos is greater than viewport size..
        if (newposx*2*static_cast<float>(viewport_.x) < -static_cast<float>(viewport_.x)) newposx = -0.5;
        if (newposy*2*static_cast<float>(viewport_.y) > static_cast<float>(viewport_.y)) newposy = 0.5;
        if (newposy*2*static_cast<float>(viewport_.y) < -static_cast<float>(viewport_.y)) newposy = -0.5;
    
        loc_ = glm::vec2(newposx, newposy);
        UpdateMVP();
    });
    calc.detach();
    render();
}

void ViewportPanel::OnMouseWheel(wxMouseEvent& e) { // todo translate by the difference
    if (isDragging_) return;
    const int scrolldelta = e.GetWheelRotation();
    
    if (scrolldelta > 0)
        zoomfactor_ *= 11.0 / 10.0;
    else
        zoomfactor_ *= 10.0 / 11.0;

    // Clamp
    zoomfactor_ = std::max(.5, std::min(2.0, zoomfactor_));
    
    view_ = scale(base_, glm::vec3(zoomfactor_, zoomfactor_, 0));
    UpdateMVP();
    render();
}

void ViewportPanel::CenterMedia() {
    loc_ = glm::vec2(0,0);
    prevpos_ = loc_;
    UpdateMVP();
    render();
}

ViewportPanel::~ViewportPanel() { // do these needs to be on the heap..?
    delete ib_;
    delete vb_;
    delete va_;
    delete layout_;
    delete shader_;
    delete renderer_;
    delete texture_;
    delete context_; // delete context last to avoid error loop
}
