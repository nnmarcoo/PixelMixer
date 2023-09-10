#pragma once
#include <wx/wx.h>

#include "ViewportPanel.h"

class ConfigPanel : public wxPanel {
public:
    ConfigPanel(wxWindow* parent);
    void BindViewport(ViewportPanel* viewport) {viewport_ = viewport;}
private:
    ViewportPanel* viewport_;
    
    void OnThresholdSliderChange(wxScrollEvent& event);
};
