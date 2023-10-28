#pragma once
#include "ViewportPanel.h"

class TextSlider;

class SettingsPanel : public wxPanel {
public:
    SettingsPanel(wxWindow* parent, ViewportPanel* viewport);
    void OnSlider(wxCommandEvent& e);
private:
    DECLARE_EVENT_TABLE()
    ViewportPanel* viewport_;

    wxStaticText* thresholdlabel_;
    TextSlider* slider_;
};
