#pragma once
#include "SettingsPanel.h"
#include "StatsPanel.h"
#include "ViewportPanel.h"

class ConfigPanel : public wxPanel {
public:
    ConfigPanel(wxWindow* parent, ViewportPanel* viewport);
    void SetTabs(short curr, short prev) const;
private:
    wxBoxSizer* mainsizer_;
    
    ViewportPanel* viewport_;
    SettingsPanel* settings_;
    StatsPanel* stats_;
};
