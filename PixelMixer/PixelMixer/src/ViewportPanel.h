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
    bool* wdragstate_; // used to disable rendering when dragging window
    wxSize viewport_;
    
    void Render();
    void OnSize(wxSizeEvent& e);
    void OnPaint(wxPaintEvent& e);
    
    bool isDragging_;
    wxPoint dragStart_;
    void OnRightDown(wxMouseEvent& e);
    void OnRightUp(wxMouseEvent& e);
    void OnDoubleLeftClick(wxMouseEvent& e);
    void OnMouseMove(wxMouseEvent& e);
    float zoomfactor_;
    void OnMouseWheel(wxMouseEvent& e);
    void UpdateMVP();
    void ResetMVP();

    void PixelSort(FrameBuffer* fb) const;
    void Preview() const;

    float positions_[16];
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
    
    glm::vec2 loc_;     // Temporary variable to store previous position during pan
    glm::vec2 prevpos_; // Last position of image on canvas as a ratio AFTER pan (initialized in center of screen)

    // simplify this ??
    glm::mat4 modl_;    // Model matrix: defines position, rotation and scale of the vertices of the model in the world.       (translation)
    glm::mat4 view_;    // View matrix: defines position and orientation of the "camera".                                      (scale)
    glm::mat4 proj_;    // Projection matrix: Maps what the "camera" sees to NDC, taking care of aspect ratio and perspective. (orthographic camera)
    glm::mat4 mvp_;     // modl_ * proj_ * view_

    int frame_;
    glm::vec2 resolution_;
    float threshold_;
    
    GLuint sqo_; // shader query object
    GLuint elapsedtime_;
    
};
