#pragma once
#include "BasicSettingsPanel.h"
#include "ConfigTabs.h"

#include "ViewportPanel.h"

class ConfigPanel : public wxPanel {
public:
    ConfigPanel(wxWindow* parent, ViewportPanel* viewport);
private:
    ViewportPanel* viewport_;
    BasicSettingsPanel* basicsettings_;
};
