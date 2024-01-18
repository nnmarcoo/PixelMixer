#pragma once
#include <wx/wx.h>
#include <GL/glew.h>
#include <wx/glcanvas.h>

class VertexBufferLayout;
class FrameBuffer;
class IndexBuffer;
class Renderer;
class Shader;
class Texture;
class VertexArray;
class VertexBuffer;
class StatsPanel;

#include "vendor/glm/gtc/matrix_transform.hpp"

typedef struct {
    glm::mat4 mvp;
    glm::vec2 location, previous_location;
    float     mx, my, vx, vy, px, py, mrx, mry,
              scale;
} preview_data;

class ViewportPanel : public wxGLCanvas {
    
public:
    ViewportPanel(wxWindow* parent, bool* dragstate);
    ~ViewportPanel() override;

    void CenterMedia();
    void ResetScale(); // TODO: adjust for scale offset
    void SetMedia(const std::string& path);
    void ExportMedia(const std::string& path) const;
    void Screenshot(const std::string& path);
    void SetThreshold(float value);
    
    void SetStatsPanel(StatsPanel* panel) { statspanel_ = panel; }

private:
    DECLARE_EVENT_TABLE()
    wxGLContext* context_;
    StatsPanel* statspanel_;
    bool* wdragstate_; // Used to disable rendering when dragging window

    preview_data preview_;
    wxSize viewport_;
    float positions_[16]; // Initial positions of preview vertices // todo: prob remove

    VertexBuffer* vb_;
    VertexArray* va_;
    IndexBuffer* ib_;
    VertexBufferLayout* layout_;
    Shader* previewshader_;
    Renderer* renderer_;
    Texture* texture_;
    
    FrameBuffer* pfb_; // preview fb
    FrameBuffer* efb_; // export fb
    Shader* step1shader_;
    
    int frame_;
    glm::vec2 resolution_;
    float threshold_;
    
    GLuint sqo_; // shader query object
    GLuint elapsedtime_;
    
    void Render();
    void OnSize(wxSizeEvent& e);
    void OnPaint(wxPaintEvent& e);
    
    bool isDragging_;
    wxPoint dragStart_; // Mouse position
    void OnRightDown(wxMouseEvent& e);
    void OnRightUp(wxMouseEvent& e);
    void OnDoubleLeftClick(wxMouseEvent& e);
    void OnMouseMove(wxMouseEvent& e);
    void OnMouseWheel(wxMouseEvent& e);

    void UpdatePosition();
    void UpdateMVP();
    void ResetMVP();

    void PixelSort(FrameBuffer* fb) const;
    void Preview() const;
    
    static void ZeroVec2(glm::vec2& vec);
    
};
