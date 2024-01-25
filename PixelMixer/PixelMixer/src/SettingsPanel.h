#pragma once
#include "ViewportPanel.h"

class TextSlider;

class SettingsPanel : public wxPanel {
public:
    SettingsPanel(wxWindow* parent, ViewportPanel* viewport);
    
private:
    DECLARE_EVENT_TABLE()
    void OnThresholdSlider(wxCommandEvent& e);
    
    ViewportPanel* viewport_;

    wxStaticText* thresholdlabel_;
    TextSlider* slider_;
};
