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
    float     mx, my,
              vx, vy,
              px, py;
} preview_data;

class ViewportPanel : public wxGLCanvas { // TODO: add a struct 'mvp' and put the values there instead of multiplying matrices?
    
public:
    ViewportPanel(wxWindow* parent, bool* DragState);
    ~ViewportPanel() override;
    wxGLContext* context_;

    void CenterMedia();
    void ResetScale();
    void SetMedia(const std::string& path);
    void ExportMedia(const std::string& path) const;
    void Screenshot(const std::string& path);
    void SetThreshold(float value);

    void SetStatsPanel(StatsPanel* statspanel) { statspanel_ = statspanel; }

private:
    DECLARE_EVENT_TABLE()
    StatsPanel* statspanel_;
    bool* wdragstate_; // Used to disable rendering when dragging window

    preview_data preview_;
    
    wxSize viewport_;     // Size of viewport
    glm::vec2 loc_;       // Position of preview in viewport
    float zoomfactor_;    // Factor to scale preview by
    float positions_[16]; // Initial positions of preview vertices // todo: prob remove
    
    void Render();
    void OnSize(wxSizeEvent& e);
    void OnPaint(wxPaintEvent& e);
    
    bool isDragging_;
    wxPoint dragStart_; // Mouse position
    glm::vec2 prevpos_; // Last position of preview  on canvas as a ratio AFTER pan (initialized in center of screen)
    void OnRightDown(wxMouseEvent& e);
    void OnRightUp(wxMouseEvent& e);
    void OnDoubleLeftClick(wxMouseEvent& e);
    void OnMouseMove(wxMouseEvent& e);
    void OnMouseWheel(wxMouseEvent& e);
    
    void UpdateMVP();
    void ResetMVP();

    void PixelSort(FrameBuffer* fb) const;
    void Preview() const;
    
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

    // Helpers
    static void ZeroVec2(glm::vec2& vec);
    
};
