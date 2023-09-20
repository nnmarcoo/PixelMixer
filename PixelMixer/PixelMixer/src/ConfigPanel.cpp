#include "ConfigPanel.h"
#include "ConfigTabs.h"
#include "StatsPanel.h"

ConfigPanel::ConfigPanel(wxWindow* parent, ViewportPanel* viewport) : wxPanel(parent), viewport_(viewport) {
    mainsizer_ = new wxBoxSizer(wxHORIZONTAL);
    auto* configtabs = new ConfigTabs(this);
    settings_ = new SettingsPanel(this, viewport_);
    stats_ = new StatsPanel(this, viewport_);
    mainsizer_->Add(configtabs, 0, wxEXPAND);
    mainsizer_->Add(settings_, 1, wxEXPAND);
    mainsizer_->Add(stats_, 1, wxEXPAND);
    stats_->Hide();
    
    SetSizerAndFit(mainsizer_);
}

void ConfigPanel::SetTabs(short curr, short prev) const {
    
    wxPanel* tabs[] = {nullptr, settings_, nullptr, stats_}; // sourcebutton_, operationbutton_, outputbutton_, statsbutton_
    tabs[prev]->Hide();
    tabs[curr]->Show();
    mainsizer_->Layout();
}
