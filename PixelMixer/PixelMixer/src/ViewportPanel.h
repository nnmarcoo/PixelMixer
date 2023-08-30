#pragma once
#include <wx/wx.h>
#include <GL/glew.h>
#include <wx/glcanvas.h>

#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

class ViewportPanel : public wxGLCanvas {
    
public:
    ViewportPanel(wxWindow* parent);
    ~ViewportPanel() override;

    wxGLContext* context_;
    
private:
    DECLARE_EVENT_TABLE()

    void render();
    void OnSize(wxSizeEvent& e);
    void OnPaint(wxPaintEvent& e);
    void OnIdle(wxIdleEvent& e);
    wxImage image_;
    
    bool initialized_;
    
    int location_;
    float r_ = 0.0f;
    float increment_ = 0.05f;
    unsigned int shader_;

    VertexBuffer* vb_;
    VertexArray* va_;
    IndexBuffer* ib_;
    VertexBufferLayout* layout_;
};
