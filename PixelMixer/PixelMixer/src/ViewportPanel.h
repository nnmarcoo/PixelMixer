#pragma once
#include <wx/wx.h>
#include <GL/glew.h>
#include <wx/glcanvas.h>

#include "IndexBuffer.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

#include "vendor/glm/gtc/matrix_transform.hpp"

class ViewportPanel : public wxGLCanvas {
    
public:
    ViewportPanel(wxWindow* parent, bool* DragState);
    ~ViewportPanel() override;

    wxGLContext* context_;
    
private:
    DECLARE_EVENT_TABLE()

    bool* wdragstate_; // used to disable rendering when dragging window
    wxSize viewport_;
    
    void render();
    void UpdateMVP();
    void OnSize(wxSizeEvent& e);
    void OnPaint(wxPaintEvent& e);
    void OnIdle(wxIdleEvent& e);
    
    bool isDragging_;
    wxPoint dragStart_;
    void OnRightDown(wxMouseEvent& e);
    void OnRightUp(wxMouseEvent& e);
    void OnDoubleLeftClick(wxMouseEvent& e);
    void OnMouseMove(wxMouseEvent& e);
    double zoomfactor_;
    void OnMouseWheel(wxMouseEvent& e);

    void CenterMedia();

    VertexBuffer* vb_;
    VertexArray* va_;
    IndexBuffer* ib_;
    VertexBufferLayout* layout_;
    Shader* shader_;
    Renderer* renderer_;
    Texture* texture_;

    
    glm::vec2 loc_;     // Temporary variable to store previous position during pan
    glm::vec2 prevpos_; // Last position of image on canvas as a ratio AFTER pan(initialized in center of screen)

    glm::mat4 base_;
    
    glm::mat4 modl_;    // Model matrix: defines position, rotation and scale of the vertices of the model in the world.
    glm::mat4 view_;    // View matrix: defines position and orientation of the "camera".
    glm::mat4 proj_;    // Projection matrix: Maps what the "camera" sees to NDC, taking care of aspect ratio and perspective.

    glm::mat4 mvp_; // modl_ * proj_ * view_
};
