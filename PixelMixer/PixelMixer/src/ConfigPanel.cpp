#include "ConfigPanel.h"
#include "ConfigTabs.h"
#include "InputPanel.h"
#include "OutputPanel.h"
#include "StatsPanel.h"

ConfigPanel::ConfigPanel(wxWindow* parent, ViewportPanel* viewport) : wxPanel(parent), viewport_(viewport) {
    mainsizer_ = new wxBoxSizer(wxHORIZONTAL);
    auto* configtabs = new ConfigTabs(this);
    settings_ = new SettingsPanel(this, viewport_);
    stats_ = new StatsPanel(this, viewport_);
    input_ = new InputPanel(this, viewport_);
    output_ = new OutputPanel(this, viewport_);
    mainsizer_->Add(configtabs, 0, wxEXPAND);
    mainsizer_->Add(settings_, 1, wxEXPAND);
    mainsizer_->Add(stats_, 1, wxEXPAND); stats_->Hide();
    mainsizer_->Add(input_, 1, wxEXPAND); input_->Hide();
    mainsizer_->Add(output_, 1, wxEXPAND); output_->Hide();
    
    SetSizerAndFit(mainsizer_);
}

void ConfigPanel::SetTabs(short curr, short prev) const {
    wxPanel* tabs[] = {input_, settings_, output_, stats_};
    tabs[prev]->Hide();
    tabs[curr]->Show();
    mainsizer_->Layout();
}
