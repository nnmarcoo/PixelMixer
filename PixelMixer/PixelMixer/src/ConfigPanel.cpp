#include "ConfigPanel.h"

ConfigPanel::ConfigPanel(wxWindow* parent) : wxPanel(parent) {

    wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);
    
    auto* configtabs = new ConfigTabs(this);

    // Create a right panel (larger)
     basicsettings_ = new BasicSettingsPanel(this);

    // Add the left and right panels to the main sizer
    mainSizer->Add(configtabs, 0, wxEXPAND);
    mainSizer->Add(basicsettings_, 1, wxEXPAND);
    
    SetSizerAndFit(mainSizer);
}