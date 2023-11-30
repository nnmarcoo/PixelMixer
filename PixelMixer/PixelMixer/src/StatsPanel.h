#pragma once
#include "wx/wx.h"

class ViewportPanel;

class StatsPanel : public wxPanel {
public:
    StatsPanel(wxWindow* parent, ViewportPanel* viewport);
    
    void UpdateRenderTime(double time) const;
    void UpdateZoomFactor(double zoomfactor) const;
    void UpdatePosition(wxPoint pos) const;
private:
    DECLARE_EVENT_TABLE()
    ViewportPanel* viewport_;
    wxStaticText* rendertime_;
    wxStaticText* rendertimevalue_;
    wxStaticText* zoomfactor_;
    wxStaticText* zoomfactorvalue_;
    wxStaticText* position_;
    wxStaticText* positionvalue_;
    
};
