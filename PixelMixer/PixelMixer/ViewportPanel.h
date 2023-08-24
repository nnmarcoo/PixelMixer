#pragma once
#include <wx/wx.h>
#include <wx/glcanvas.h>

class ViewportPanel : public wxGLCanvas {
    wxGLContext* context;
public:
    ViewportPanel(wxWindow* parent);
    ~ViewportPanel() override;

    void render(wxPaintEvent& e);
    wxImage image;
    
    DECLARE_EVENT_TABLE()
};