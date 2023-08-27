#include "ConfigPanel.h"

ConfigPanel::ConfigPanel(wxWindow *parent) : wxPanel(parent) {
    wxWindowBase::SetBackgroundColour("#282b30");

    wxStaticText* threshold = new wxStaticText(this, wxID_ANY, "Threshold", wxPoint(30,30));
    threshold->SetForegroundColour("#bfbfbf");
    threshold->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
}