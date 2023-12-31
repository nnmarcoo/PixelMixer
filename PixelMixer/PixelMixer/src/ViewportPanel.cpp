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

ViewportPanel::ViewportPanel(wxWindow* parent, bool* dragstate) : wxGLCanvas(parent, wxID_ANY, nullptr, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE), wdragstate_(dragstate) {
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
    GLCall(glClearColor(0.19140625f, 0.19921875f, 0.21875f, 1.0))   // Palette::viewport

    glGenQueries(1, &sqo_);

    preview_.scale = 1.0f;
    preview_.mx = preview_.my = preview_.vx = preview_.vy = preview_.px = preview_.py = 1;
    preview_.mvp[3][3] = 1.0f;

    threshold_ = 0.5f;
    
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
    
    previewshader_ = new Shader("res/shaders/DisplayPreview.glsl");
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

    statspanel_->UpdateZoomFactor(preview_.scale);
    statspanel_->UpdateRenderTime(static_cast<double>(time) * 1.0e-6);
    statspanel_->UpdatePosition(static_cast<int>(preview_.location.x)/2, static_cast<int>(preview_.location.y)/2);
    
    SwapBuffers();
}

void ViewportPanel::OnPaint(wxPaintEvent& e) {
    if (*wdragstate_) return;
    Render();
}

void ViewportPanel::OnSize(wxSizeEvent& e) {
    viewport_ = GetSize();
    if (viewport_.x < 1) return;

    if (preview_.location.x > static_cast<float>(viewport_.x)  || // TODO: animate it back instead of snap?
        preview_.location.x < -static_cast<float>(viewport_.x) ||
        preview_.location.y > static_cast<float>(viewport_.y)  ||
        preview_.location.y < -static_cast<float>(viewport_.x)  )
        CenterMedia();
    
    glViewport(0, 0, viewport_.x, viewport_.y);
    preview_.px = 2 / (static_cast<float>(viewport_.x) + static_cast<float>(viewport_.x));
    preview_.py = 2 / (static_cast<float>(viewport_.y) + static_cast<float>(viewport_.y));
    
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
    preview_.previous_location = preview_.location;
}

void ViewportPanel::OnDoubleLeftClick(wxMouseEvent& e) {
    if (preview_.mx == 0.0f && preview_.my == 0.0f) return;
    CenterMedia();
}

void ViewportPanel::OnMouseMove(wxMouseEvent& e) {
    if (!isDragging_) return;
    
    const wxPoint delta = (e.GetPosition() - dragStart_) * 2;
    
    preview_.location.x = static_cast<float>(delta.x) + preview_.previous_location.x;
    preview_.location.y = static_cast<float>(delta.y) + preview_.previous_location.y;
    
    UpdateMVP();
    Render();
}

void ViewportPanel::OnMouseWheel(wxMouseEvent& e) { // todo translate so the mouse is centered
    if (isDragging_) return;
    constexpr float max = 20.0f, min = 0.00001f;
    
    const float prevzoomval = preview_.mvp[0][0] * preview_.scale;
    
    preview_.scale *= static_cast<float>(e.GetWheelRotation() > 0 ? 11.0 / 10.0 : 10.0 / 11.0);
    
    const float zoomval = preview_.mvp[0][0] * preview_.scale;
    const float diff = zoomval - prevzoomval;

    if (!((diff < 0 && prevzoomval > max) || (diff > 0 && prevzoomval < min)) && (zoomval > max || zoomval < min)) { // If the resulting zoom does 
        preview_.scale = preview_.scale *= static_cast<float>(e.GetWheelRotation() < 0 ? 11.0 / 10.0 : 10.0 / 11.0); // NOT APPROACH the range, undo it
        return;
    }

    preview_.vx = preview_.vy = preview_.scale;
    UpdateMVP();
    Render();
}

void ViewportPanel::UpdatePosition() { // TODO: this needs to incorporate the offset from the mouse
    preview_.mx = preview_.location.x / preview_.scale; // TODO: offset by relative pos of mouse
    preview_.my = preview_.location.y / preview_.scale;
}

void ViewportPanel::UpdateMVP() {
    UpdatePosition();
    
    preview_.mvp[0][0] =  preview_.px * preview_.vx;
    preview_.mvp[1][1] =  preview_.py * preview_.vy;
    preview_.mvp[3][0] =  preview_.mx * preview_.mvp[0][0];
    preview_.mvp[3][1] = -preview_.my * preview_.mvp[1][1];
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
    previewshader_->SetUniformMat4f("u_MVP", preview_.mvp);

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
    preview_.scale = 1.0f;
    preview_.vx = preview_.vy = 1.0f;
    CenterMedia();
}

void ViewportPanel::CenterMedia() {
    ZeroVec2(preview_.location);
    ZeroVec2(preview_.previous_location);
    UpdateMVP();
    Render();
}

void ViewportPanel::ResetScale() {
    preview_.scale = 1.0f;
    preview_.vx = preview_.vy = 1.0f;
    CenterMedia();
}

void ViewportPanel::SetThreshold(const float value) {
    threshold_ = value;
    Render();
}

void ViewportPanel::ZeroVec2(glm::vec2& vec) { vec.x = vec.y = 0.0f; }

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
