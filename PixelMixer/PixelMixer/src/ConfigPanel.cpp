#include "ConfigPanel.h"
#include "ConfigTabs.h"

ConfigPanel::ConfigPanel(wxWindow* parent, ViewportPanel* viewport) : wxPanel(parent), viewport_(viewport) {
    mainsizer_ = new wxBoxSizer(wxHORIZONTAL);
    auto* configtabs = new ConfigTabs(this);
    settings_ = new SettingsPanel(this, viewport_);
    mainsizer_->Add(configtabs, 0, wxEXPAND);
    mainsizer_->Add(settings_, 1, wxEXPAND);
    
    SetSizerAndFit(mainsizer_);
}

void ConfigPanel::SetTabs(short curr, short prev) const {
    
    wxPanel* tabs[] = {0, settings_, 0, 0}; // sourcebutton_, operationbutton_, outputbutton_, statsbutton_
    tabs[curr]->Show();
    tabs[prev]->Hide();
}
