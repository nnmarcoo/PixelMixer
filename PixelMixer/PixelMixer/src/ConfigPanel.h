#pragma once

#include <wx/wx.h>

class SettingsPanel;
class InputPanel;
class OutputPanel;
class ViewportPanel;
class StatsPanel;

class ConfigPanel : public wxPanel {
public:
    ConfigPanel(wxWindow* parent, ViewportPanel* viewport);
    void SetTabs(short curr, short prev) const;
private:
    wxBoxSizer* mainsizer_;
    
    ViewportPanel* viewport_;
    SettingsPanel* settings_;
    StatsPanel* stats_;
    InputPanel* input_;
    OutputPanel* output_;
};
