#include "ConfigPanel.h"

ConfigPanel::ConfigPanel(wxWindow* parent) : wxPanel(parent) {
    wxBoxSizer* mainsizer = new wxBoxSizer(wxHORIZONTAL);
    auto* configtabs = new ConfigTabs(this);
    basicsettings_ = new BasicSettingsPanel(this);
    mainsizer->Add(configtabs, 0, wxEXPAND);
    mainsizer->Add(basicsettings_, 1, wxEXPAND);
    
    SetSizerAndFit(mainsizer);
}