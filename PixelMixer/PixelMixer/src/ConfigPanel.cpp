#include "ConfigPanel.h"

ConfigPanel::ConfigPanel(wxWindow* parent) : wxPanel(parent) {
    wxWindowBase::SetBackgroundColour("#282b30");

    // Create a StaticText label for the slider
    wxStaticText* thresholdLabel = new wxStaticText(this, wxID_ANY, "Threshold", wxPoint(30, 30));
    thresholdLabel->SetForegroundColour("#bfbfbf");
    thresholdLabel->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    
    wxSlider* thresholdSlider = new wxSlider(this, wxID_ANY, 127, 0, 255, wxPoint(30, 60), wxSize(200, -1), wxSL_HORIZONTAL);
    
    thresholdSlider->Bind(wxEVT_SCROLL_THUMBTRACK, &ConfigPanel::OnThresholdSliderChange, this);
}

void ConfigPanel::OnThresholdSliderChange(wxScrollEvent& event) {
     viewport_->SetThreshold(static_cast<float>(event.GetPosition()) / static_cast<float>(255.0));
}
