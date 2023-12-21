#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "ViewportPanel.h"

#include <wx/display.h>

#include "FrameBuffer.h"
#include "IndexBuffer.h"
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

ViewportPanel::ViewportPanel(wxWindow* parent, bool* DragState) : wxGLCanvas(parent, wxID_ANY, nullptr, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE), wdragstate_(DragState) {
    context_ = new wxGLContext(this);
    SetCurrent(*context_);

    const GLenum glewInitResult = glewInit();
    if (glewInitResult != GLEW_OK) {
        std::cerr << "GLEW initialization failed: " << glewGetErrorString(glewInitResult) << std::endl;
        return;
    }
    std::cout << glGetString(GL_VERSION)  << '\n' <<
                 glGetString(GL_RENDERER) << '\n' <<
                 glGetString(GL_VENDOR)   << '\n' << std::endl;

    GLCall(glEnable(GL_BLEND))                                      // Enable blending
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA))       // Blend the alpha channel
    GLCall(glClearColor(0.19140625f, 0.19921875f, 0.21875f, 1.0))   // Set clear color to Palette::viewport

    glGenQueries(1, &sqo_);

    zoomfactor_ = 1.0f;
    view_ = scale(glm::mat4(1.0f), glm::vec3(1, 1, 0));
    modl_ = scale(glm::mat4(1.0f), glm::vec3(1, 1, 0));

    threshold_ = 0.5;
    
    /* REST OF CONSTRUCTOR IS FOR TESTING */

    constexpr float s = 500;
    constexpr float positions[] = {
         -1.0f*s, -1.0f*s, 0.0f, 0.0f, // 0 bottom-left
          1.0f*s, -1.0f*s, 1.0f, 0.0f, // 1 bottom-right
          1.0f*s,  1.0f*s, 1.0f, 1.0f, // 2 top-right
         -1.0f*s,  1.0f*s, 0.0f, 1.0f  // 3 top-left
    };
    memcpy(positions_, positions, sizeof(float) * 16); // is there a better solution?

    const unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    
    vb_ = new VertexBuffer(positions, static_cast<unsigned long long>(4) * 4 * sizeof(float)); // points * components * how big each component is
    va_ = new VertexArray();
    layout_ = new VertexBufferLayout();
    
    layout_->Push<float>(2);
    layout_->Push<float>(2);
    va_->AddBuffer(*vb_, *layout_);
    va_->Bind();

    ib_ = new IndexBuffer(indices, 6);
    ib_->Bind();
    
    previewshader_ = new Shader("res/shaders/Display.glsl");
    step1shader_ = new Shader("res/shaders/Step1.glsl");
    
    texture_ = new Texture("res/textures/debug.jpg");
    pfb_ = new FrameBuffer(texture_->GetWidth(), texture_->GetHeight());
}

void ViewportPanel::Render() {
    if (!IsShown()) return;
    
    frame_++;
    glBeginQuery(GL_TIME_ELAPSED, sqo_);
    
    PixelSort(pfb_);
    Preview();
    
    glEndQuery(GL_TIME_ELAPSED);
    glGetQueryObjectuiv(sqo_, GL_QUERY_RESULT_AVAILABLE, &elapsedtime_);
    GLuint64 time;
    glGetQueryObjectui64v(sqo_, GL_QUERY_RESULT, &time);

    statspanel_->UpdateZoomFactor(zoomfactor_);
    statspanel_->UpdateRenderTime(static_cast<double>(time) * 1.0e-6);
    statspanel_->UpdatePosition(static_cast<int>(loc_.x * static_cast<float>(viewport_.x)),
                                static_cast<int>(loc_.y * static_cast<float>(viewport_.y)));
    SwapBuffers();
}

void ViewportPanel::OnPaint(wxPaintEvent& e) {
    if (*wdragstate_) return;
    Render();
}

void ViewportPanel::OnSize(wxSizeEvent& e) {
    viewport_ = GetSize();
    if (viewport_.x < 1) return;
    
    glViewport(0, 0, viewport_.x, viewport_.y);
    proj_ = glm::ortho(-static_cast<float>(viewport_.x), static_cast<float>(viewport_.x), -static_cast<float>(viewport_.y), static_cast<float>(viewport_.y), -1.0f, 1.0f);

    UpdateMVP();
    resolution_ = glm::vec2(viewport_.x, viewport_.y); // this should be no longer necessary cuz of pfb_?
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
    if (modl_[3][0] == 0.0f && modl_[3][1] == 0.0f) return;
    CenterMedia();
}

void ViewportPanel::OnMouseMove(wxMouseEvent& e) {
    if (!isDragging_) return;
    
    const wxPoint delta = e.GetPosition() - dragStart_;
    const float ratiox = static_cast<float>(delta.x) / static_cast<float>(viewport_.x);
    const float ratioy = static_cast<float>(delta.y) / static_cast<float>(viewport_.y);
    
    loc_.x = ratiox + prevpos_.x;
    loc_.y = ratioy + prevpos_.y;
    
    UpdateMVP();
    Render();
}

void ViewportPanel::OnMouseWheel(wxMouseEvent& e) { // todo translate so the mouse is centered
    if (isDragging_) return;
    constexpr float max = 20, min = 0.00001f;

    glm::vec4 prevpos = glm::vec4(positions_[0], positions_[1], 1, 1) * mvp_;
    const float prevzoomval = mvp_[0][0] * zoomfactor_;
    
    zoomfactor_ *= static_cast<float>(e.GetWheelRotation() > 0 ? 11.0 / 10.0 : 10.0 / 11.0);
    
    const float zoomval = mvp_[0][0] * zoomfactor_;
    const float diff = zoomval - prevzoomval;

    if (!((diff < 0 && prevzoomval > max) || (diff > 0 && prevzoomval < min)) && (zoomval > max || zoomval < min)) { // If the resulting zoom does 
        zoomfactor_ = zoomfactor_ *= static_cast<float>(e.GetWheelRotation() < 0 ? 11.0 / 10.0 : 10.0 / 11.0);       // NOT APPROACH the range, undo it
        return;
    }
    
    view_[0][0] = zoomfactor_;
    view_[1][1] = zoomfactor_;
    UpdateMVP(); // shouldn't this always be last?

    glm::vec4 pos = glm::vec4(positions_[0], positions_[1], 1, 1) * mvp_;
    float diffposx =  abs((pos.x - prevpos.x) / 2);
    float diffposy =  abs((pos.y - prevpos.y) / 2);
    wxPoint mousepos = e.GetPosition() - wxPoint(viewport_.x / 2, viewport_.y / 2);

    view_[3][0] = diffposx;
    view_[3][1] = diffposy;
    UpdateMVP();
    Render();
}

void ViewportPanel::UpdateMVP() {
    modl_[3][0] =  loc_.x * static_cast<float>(viewport_.x) * (2 / zoomfactor_);
    modl_[3][1] = -loc_.y * static_cast<float>(viewport_.y) * (2 / zoomfactor_);
    mvp_ = proj_ * view_ * modl_;
}

void ViewportPanel::PixelSort(FrameBuffer* fb) const { // TODO: more steps.. actually sort
    glViewport(0, 0, static_cast<int>(fb->GetWidth()), static_cast<int>(fb->GetHeight()));
    fb->Bind();
    step1shader_->Bind();
    step1shader_->SetUniform1i("u_Texture", 0);
    texture_->Bind();
    
    Renderer::Clear();
    Renderer::Draw(*step1shader_);
}

void ViewportPanel::Preview() const {
    glViewport(0, 0, viewport_.x, viewport_.y);
    pfb_->Unbind();
    pfb_->GetTexture()->Bind();
    previewshader_->Bind();
    previewshader_->SetUniform1i("u_Texture", 0);
    previewshader_->SetUniformMat4f("u_MVP", mvp_);

    Renderer::Clear();
    Renderer::Draw(*va_, *ib_, *previewshader_);
}

void ViewportPanel::SetMedia(const std::string& path) {
    frame_ = 0;

    texture_ = new Texture(path);
    texture_->Bind();
    
    float distx = static_cast<float>(texture_->GetWidth() >> 1);
    float disty = static_cast<float>(texture_->GetHeight() >> 1);

    while (distx < viewport_.x-40.0 && disty < viewport_.y-40.0) {
        distx+=40;
        disty+=40;
    }
    while (distx > viewport_.x-40.0 && disty > viewport_.y-40.0) {
        distx-=40;
        disty-=40;
    }

    const float positions[] = {
        -1.0f*distx, -1.0f*disty, 0.0f, 0.0f, // 0 bottom-left
         1.0f*distx, -1.0f*disty, 1.0f, 0.0f, // 1 bottom-right
         1.0f*distx,  1.0f*disty, 1.0f, 1.0f, // 2 top-right
        -1.0f*distx,  1.0f*disty, 0.0f, 1.0f  // 3 top-left
    };
    memcpy(positions_, positions, sizeof(float) * 16);

    vb_ = new VertexBuffer(positions, static_cast<unsigned long long>(4) * 4 * sizeof(float)); // points * components * how big each component is // why am I casting
    va_ = new VertexArray();
    layout_ = new VertexBufferLayout();
    
    layout_->Push<float>(2);
    layout_->Push<float>(2);
    va_->AddBuffer(*vb_, *layout_);
    va_->Bind();
    
    previewshader_->SetUniform1i("u_Texture", 0);
    
    pfb_ = new FrameBuffer(texture_->GetWidth(), texture_->GetHeight());
    texture_->Bind();
    
    ResetMVP();
    Render();
}

void ViewportPanel::ExportMedia(const std::string& path) const { // TODO: change to pixel sort then pull from efb_

    const int width = texture_->GetWidth();
    const int height = texture_->GetHeight();

    pfb_->GetTexture()->Bind();
    std::vector<unsigned char> data(static_cast<unsigned long long>(width * height * 4));
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
    stbi_flip_vertically_on_write(1);
    stbi_write_png(path.c_str(), width, height, 4, data.data(), width * 4);
    
    glBindTexture(GL_TEXTURE_2D, 0);
}

void ViewportPanel::Screenshot(const std::string& path) { // todo put in clipboard?

    const int width = viewport_.x;
    const int height = viewport_.y;
    
    std::vector<unsigned char> data(static_cast<unsigned long long>(width) * static_cast<unsigned long long>(height) * 4);
    
    Render();
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data.data());

    stbi_flip_vertically_on_write(1);
    stbi_write_png(path.c_str(), width, height, 4, data.data(), 4 * width);
    stbi_flip_vertically_on_write(0);
}

void ViewportPanel::ResetMVP() {
    zoomfactor_ = 1.0f;
    view_ = scale(glm::mat4(1.0f), glm::vec3(1, 1, 0));
    CenterMedia();
}

void ViewportPanel::CenterMedia() {
    ZeroVec2(loc_);
    ZeroVec2(prevpos_);
    UpdateMVP();
    Render();
}

void ViewportPanel::ResetScale() {
    zoomfactor_ = 1.0f;
    view_ = scale(glm::mat4(1.0f), glm::vec3(zoomfactor_, zoomfactor_, 0));
    CenterMedia();
}

void ViewportPanel::SetThreshold(const float value) {
    threshold_ = value;
    Render();
}

void ViewportPanel::ZeroVec2(glm::vec2& vec) { vec.x = vec.y = 0; }

ViewportPanel::~ViewportPanel() {
    delete ib_;
    delete vb_;
    delete va_;
    delete pfb_;
    delete layout_;
    delete previewshader_;
    delete renderer_;
    delete texture_;
    delete context_; // delete context last to avoid error loop
}
