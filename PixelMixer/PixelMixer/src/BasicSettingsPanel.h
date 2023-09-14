#pragma once
#include "ViewportPanel.h"

class BasicSettingsPanel : public wxPanel {
public:
    BasicSettingsPanel(wxWindow* parent);
    void BindViewport(ViewportPanel* viewport) {viewport_ = viewport;}
private:
    void OnThresholdSliderChange(wxScrollEvent& e);
    ViewportPanel* viewport_;
};
