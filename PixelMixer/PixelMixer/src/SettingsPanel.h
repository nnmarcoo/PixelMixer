#pragma once
#include "ViewportPanel.h"

class TextSlider;

class SettingsPanel : public wxPanel {
public:
    SettingsPanel(wxWindow* parent, ViewportPanel* viewport);
private:
    void OnThresholdSliderChange(wxScrollEvent& e);
    ViewportPanel* viewport_;

    TextSlider* slider_;
};
