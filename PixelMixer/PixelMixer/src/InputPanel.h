#pragma once
#include "TextSlider.h"
#include "ViewportPanel.h"

class InputPanel : public wxPanel {
public:
    InputPanel(wxWindow* parent, ViewportPanel* viewport);
private:
    DECLARE_EVENT_TABLE()
    void OnScaleSlider(wxCommandEvent& e);
    ViewportPanel* viewport_;
    
    wxStaticText* scalelabel_;
    TextSlider* scaleslider_;
};
