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
    
    wxImage image_;
    
    float r_ = 0.0f;
    float increment_ = 0.05f;

    VertexBuffer* vb_;
    VertexArray* va_;
    IndexBuffer* ib_;
    VertexBufferLayout* layout_;
    Shader* shader_;
    Renderer* renderer_;
    Texture* texture_;
    
    glm::mat4 modl_ = translate(glm::mat4(1.0f), glm::vec3(0,0,0)); // Model matrix: defines position, rotation and scale of the vertices of the model in the world.
    // View matrix: defines position and orientation of the "camera".
    glm::mat4 proj_; // Projection matrix: Maps what the "camera" sees to NDC, taking care of aspect ratio and perspective.

    glm::mat4 mvp_; // modl_ * proj_ * view_
};
