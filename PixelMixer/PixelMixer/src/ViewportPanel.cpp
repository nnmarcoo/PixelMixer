#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "ViewportPanel.h"

#include <wx/display.h>

#include "FrameBuffer.h"
#include "IndexBuffer.h"
#include "Palette.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

#include "VertexBufferLayout.h"

#include "vendor/glm/gtx/string_cast.hpp"
#include "vendor/glm/glm.hpp"
#include "vendor/stb_image/stb_image.h"
#include "vendor/stb_image/stb_image_write.h"

#include "StatsPanel.h"

//todo waiting for media and animation when no media
//todo if a button is held, it will show the original image
//todo if portion of viewport is off screen, it won't be updated if dragged back into screen until later

BEGIN_EVENT_TABLE(ViewportPanel, wxGLCanvas)
    EVT_PAINT(ViewportPanel::OnPaint)
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
    std::cout << glGetString(GL_VERSION) << '\n' << glGetString(GL_RENDERER) << '\n' << glGetString(GL_VENDOR) << '\n' << std::endl; // debug

    GLCall(glEnable(GL_BLEND))                                      // Enable blending
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA))       // Blend the alpha channel
    GLCall(glClearColor(0.19140625f, 0.19921875f, 0.21875f, 1.0))  // Set clear color to Palette::viewport

    glGenQueries(1, &sqo_);

    zoomfactor_ = 1.0f;
    base_ = glm::mat4(1.0f);
    view_ = scale(base_, glm::vec3(1, 1, 0));

    threshold_ = 0.5;
    
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
    
    vb_ = new VertexBuffer(positions, static_cast<unsigned long long>(4) * 4 * sizeof(float)); // points * components * how big each component is // WHY AM I CASTING
    va_ = new VertexArray();
    layout_ = new VertexBufferLayout();
    
    layout_->Push<float>(2);
    layout_->Push<float>(2);
    va_->AddBuffer(*vb_, *layout_);
    va_->Bind();

    ib_ = new IndexBuffer(indices, 6);
    ib_->Bind();
    
    shader_ = new Shader("res/shaders/SimpleSort.glsl");
    sshader_ = new Shader("res/shaders/Display.glsl");

    shader_->Bind();

    sfb_ = new FrameBuffer(2048, 2048);
    
    texture_ = new Texture("res/textures/debug.jpg");
    texture_->Bind();
    shader_->SetUniform1i("u_Texture", 0);
}

void ViewportPanel::render() {
    if (!IsShown()) return;
    frame_++;
    glBeginQuery(GL_TIME_ELAPSED, sqo_);
    
    Renderer::Clear();
    shader_->SetUniformMat4f("u_MVP", mvp_);

    sfb_->Bind();
    Renderer::Draw(*va_, *ib_, *shader_);
    sfb_->Unbind();

    Renderer::Draw(*va_, *ib_, *shader_);
    
    glEndQuery(GL_TIME_ELAPSED);
    glGetQueryObjectuiv(sqo_, GL_QUERY_RESULT_AVAILABLE, &elapsedtime_);
    GLuint64 shaderExecutionTime;
    glGetQueryObjectui64v(sqo_, GL_QUERY_RESULT, &shaderExecutionTime);

    statspanel_->UpdateRenderTime(static_cast<double>(shaderExecutionTime) * 1.0e-6);
    statspanel_->UpdateZoomFactor(zoomfactor_);
    
    SwapBuffers();

    // Debug
    std::cout << threshold_ << std::endl;
}

/* MEDIA CONTROLS */

void ViewportPanel::OnPaint(wxPaintEvent& e) {
    if (*wdragstate_) return;
    render();
}

void ViewportPanel::OnSize(wxSizeEvent& e) {
    viewport_ = GetSize();
    if (viewport_.x < 1) return;
    
    glViewport(0, 0, viewport_.x, viewport_.y);
    proj_ = glm::ortho(-static_cast<float>(viewport_.x), static_cast<float>(viewport_.x), -static_cast<float>(viewport_.y), static_cast<float>(viewport_.y), -1.0f, 1.0f);
    UpdateMVP();
    resolution_ = glm::vec2(viewport_.x, viewport_.y);
    
}

void ViewportPanel::OnRightDown(wxMouseEvent& e) {
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
    if (modl_[3][0] == 0.0 && modl_[3][1] == 0.0) return;
    CenterMedia();
}

void ViewportPanel::OnMouseMove(wxMouseEvent& e) {
    if (!isDragging_) return;
    /*
    std::thread calc([this, e] {

    });
    calc.detach();
    */
    const wxPoint delta = e.GetPosition() - dragStart_;
    const float ratiox = static_cast<float>(delta.x) / static_cast<float>(viewport_.x);
    const float ratioy = static_cast<float>(delta.y) / static_cast<float>(viewport_.y);

    float newposx = ratiox + prevpos_.x;
    float newposy = ratioy + prevpos_.y;

    loc_ = glm::vec2(newposx, newposy);
    UpdateMVP();
    render();
}

void ViewportPanel::OnMouseWheel(wxMouseEvent& e) { // todo translate so the mouse is centered
    if (isDragging_) return;
    const double MAX = 20, MIN = 0.00001;
    
    const double prevzoomval = mvp_[0][0] * zoomfactor_;
    zoomfactor_ *= e.GetWheelRotation() > 0 ? 11.0 / 10.0 : 10.0 / 11.0;
    const double zoomval = mvp_[0][0] * zoomfactor_;
    const double diff = zoomval - prevzoomval;

    if (!((diff < 0 && prevzoomval > MAX) || (diff > 0 && prevzoomval < MIN)) && (zoomval > MAX || zoomval < MIN)) { // If the resulting zoom does 
        zoomfactor_ = zoomfactor_ *= e.GetWheelRotation() < 0 ? 11.0 / 10.0 : 10.0 / 11.0;                           // NOT APPROACH the range, undo it
        return;
    }
    
    view_ = scale(base_, glm::vec3(zoomfactor_, zoomfactor_, 0));
    UpdateMVP();
    render();
}

void ViewportPanel::UpdateMVP() {
    modl_ = translate(base_, glm::vec3(loc_.x * static_cast<float>(viewport_.x) * 2 * (1 / zoomfactor_), // modl_ must be recalculated for all window manipulation
                                     -loc_.y * static_cast<float>(viewport_.y) * 2 * (1 / zoomfactor_), 0));
    mvp_ = proj_ * view_ * modl_;
}

void ViewportPanel::ResetMVP() {
    zoomfactor_ = 1.0f;
    view_ = base_;
    CenterMedia();
    UpdateMVP();
}

void ViewportPanel::CenterMedia() {
    if (loc_ == glm::vec2(0,0)) return;
    loc_ = glm::vec2(0,0);
    prevpos_ = loc_;
    UpdateMVP();
    render();
}

void ViewportPanel::ResetScale() { // todo: instead of centering the media, use the current zoom point as the origin and keep it in the same spot when it zooms out
    zoomfactor_ = 1.0f;
    view_ = scale(base_, glm::vec3(zoomfactor_, zoomfactor_, 0));
    CenterMedia(); 
}

void ViewportPanel::SetMedia(const std::string& path) {
    frame_ = 0;
    const wxSize img = wxImage(path).GetSize(); // todo bad
    auto distx = static_cast<float>(img.x >> 1);
    auto disty = static_cast<float>(img.y >> 1);

    while (distx < viewport_.x-40.0 && disty < viewport_.y-40.0) {
        distx+=40;
        disty+=40;
    }
    while (distx > viewport_.x-40.0 && disty > viewport_.y-40.0) {
        distx-=40;
        disty-=40;
    }
    
    float positions[] = {
        -1.0f*distx, -1.0f*disty, 0.0f, 0.0f, // 0 bottom-left
         1.0f*distx, -1.0f*disty, 1.0f, 0.0f, // 1 bottom-right
         1.0f*distx,  1.0f*disty, 1.0f, 1.0f, // 2 top-right
        -1.0f*distx,  1.0f*disty, 0.0f, 1.0f  // 3 top-left
   };

    vb_ = new VertexBuffer(positions, static_cast<unsigned long long>(4) * 4 * sizeof(float)); // points * components * how big each component is // why am I casting
    va_ = new VertexArray();
    layout_ = new VertexBufferLayout();
    
    layout_->Push<float>(2);
    layout_->Push<float>(2);
    va_->AddBuffer(*vb_, *layout_);
    va_->Bind();

    texture_ = new Texture(path);
    texture_->Bind();
    shader_->SetUniform1i("u_Texture", 0);
    
    sfb_ = new FrameBuffer(img.x, img.y);
    texture_->Bind();
    
    ResetMVP();
    render();
}

void ViewportPanel::ExportMedia(const std::string& path) {
    
}

void ViewportPanel::Screenshot(const std::string& path) { // todo put in clipboard?
    std::vector<unsigned char> data(static_cast<unsigned long long>(viewport_.x) * static_cast<unsigned long long>(viewport_.y) * 4); // why am I casting
    
    render();
    glReadPixels(0, 0, viewport_.x, viewport_.y, GL_RGBA, GL_UNSIGNED_BYTE, data.data());

    stbi_flip_vertically_on_write(1);
    stbi_write_png(path.c_str(), viewport_.x, viewport_.y, 4, data.data(), 4 * viewport_.x);
    stbi_flip_vertically_on_write(0);
}

void ViewportPanel::SetThreshold(float value) {
    threshold_ = value;
    render();
}

ViewportPanel::~ViewportPanel() { // do these needs to be on the heap..?
    delete ib_;
    delete vb_;
    delete va_;
    delete sfb_;
    delete layout_;
    delete shader_;
    delete renderer_;
    delete texture_;
    delete context_; // delete context last to avoid error loop
}
