#pragma once
#include "ViewportPanel.h"

class BasicSettingsPanel : public wxPanel {
public:
    BasicSettingsPanel(wxWindow* parent, ViewportPanel* viewport);
private:
    void OnThresholdSliderChange(wxScrollEvent& e);
    ViewportPanel* viewport_;
};
