#include "SettingsPanel.h"
#include "Palette.h"
#include "TextSlider.h"

BEGIN_EVENT_TABLE(SettingsPanel, wxPanel)
    EVT_TEXT(wxID_ANY, SettingsPanel::OnSlider)
END_EVENT_TABLE()

SettingsPanel::SettingsPanel(wxWindow* parent, ViewportPanel* viewport) : wxPanel(parent), viewport_(viewport) {
    wxWindowBase::SetBackgroundColour(Palette::config);
    
    thresholdlabel_ = new wxStaticText(this, wxID_ANY, "Threshold", wxPoint(30, 30));
    thresholdlabel_->SetForegroundColour(Palette::text);
    thresholdlabel_->SetFont(wxFont(13, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    slider_ = new TextSlider(this, wxID_ANY, "50.00", 0 , 100, wxPoint(150, 30));
}


void SettingsPanel::OnSlider(wxCommandEvent& e) {
    viewport_->SetThreshold(slider_->Value() / 100);
}