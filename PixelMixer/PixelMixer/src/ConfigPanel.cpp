#include "ConfigPanel.h"

ConfigPanel::ConfigPanel(wxWindow* parent, ViewportPanel* viewport) : wxPanel(parent), viewport_(viewport) {
    wxBoxSizer* mainsizer = new wxBoxSizer(wxHORIZONTAL);
    auto* configtabs = new ConfigTabs(this);
    basicsettings_ = new BasicSettingsPanel(this, viewport_);
    mainsizer->Add(configtabs, 0, wxEXPAND);
    mainsizer->Add(basicsettings_, 1, wxEXPAND);
    
    SetSizerAndFit(mainsizer);
}