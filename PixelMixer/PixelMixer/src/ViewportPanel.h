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
    void OnMouseMove(wxMouseEvent& e);
    void OnMouseWheel(wxMouseEvent& e);
    
    wxImage image_;

    VertexBuffer* vb_;
    VertexArray* va_;
    IndexBuffer* ib_;
    VertexBufferLayout* layout_;
    Shader* shader_;
    Renderer* renderer_;
    Texture* texture_;

    float zoomfactor_;
    
    glm::vec2 loc_; // Current position of image on canvas as a ratio
    glm::vec2 prevpos_; // Previous position of image on canvas as a ratio (initialized in center of screen)
    glm::mat4 modl_; // Model matrix: defines position, rotation and scale of the vertices of the model in the world.
    // View matrix: defines position and orientation of the "camera".
    glm::mat4 proj_; // Projection matrix: Maps what the "camera" sees to NDC, taking care of aspect ratio and perspective.

    glm::mat4 mvp_; // modl_ * proj_ * view_
};
