#pragma once
#include "TextSlider.h"
#include "ViewportPanel.h"

class InputPanel : public wxPanel {
public:
    InputPanel(wxWindow* parent, ViewportPanel* viewport);
private:
    ViewportPanel* viewport_;
    
    wxStaticText* scalelabel_;
    TextSlider* scaleslider_;
};
