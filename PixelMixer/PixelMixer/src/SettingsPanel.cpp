#include "SettingsPanel.h"
#include "Palette.h"
#include "TextSlider.h"

SettingsPanel::SettingsPanel(wxWindow* parent, ViewportPanel* viewport) : wxPanel(parent), viewport_(viewport) {
    wxWindowBase::SetBackgroundColour(Palette::config);

    // Create a StaticText label for the slider
    wxStaticText* thresholdLabel = new wxStaticText(this, wxID_ANY, "Threshold", wxPoint(30, 30));
    thresholdLabel->SetForegroundColour(Palette::text);
    thresholdLabel->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    
    wxSlider* thresholdSlider = new wxSlider(this, wxID_ANY, 127, 0, 255, wxPoint(30, 60), wxSize(200, -1));
    
    thresholdSlider->Bind(wxEVT_SCROLL_THUMBTRACK, &SettingsPanel::OnThresholdSliderChange, this);

    slider_ = new TextSlider(this, wxID_ANY, "50", 0 , 100);
}

void SettingsPanel::OnThresholdSliderChange(wxScrollEvent& e) {
    viewport_->SetThreshold(static_cast<float>(e.GetPosition()) / static_cast<float>(255.0));
    
}