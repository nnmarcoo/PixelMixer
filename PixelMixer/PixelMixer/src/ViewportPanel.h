#pragma once
#include <wx/wx.h>
#include <wx/glcanvas.h>

class ViewportPanel : public wxGLCanvas {
    wxGLContext* context;
public:
    ViewportPanel(wxWindow* parent);
    ~ViewportPanel() override;
    
private:
    DECLARE_EVENT_TABLE()

    void render();
    void OnSize(wxSizeEvent& e);
    void OnPaint(wxPaintEvent& e);
    void OnIdle(wxIdleEvent& e);
    wxImage image;
    
    bool initialized;
    
    int location;
    float r = 0.0f;
    float increment = 0.05f;
};