#pragma once
#include <wx/wx.h>
#include <wx/glcanvas.h>
#include "App.h"

class ViewportPanel : public wxGLCanvas {
    wxGLContext* context;
public:
    ViewportPanel(wxWindow* parent);
    ~ViewportPanel() override;

    void OnSize(wxSizeEvent& e);
    void render(wxPaintEvent& e);
    wxImage image;
    
    DECLARE_EVENT_TABLE()
    bool initialized;
};