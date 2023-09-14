#pragma once
#include "BasicSettingsPanel.h"
#include "ConfigTabs.h"

#include "ViewportPanel.h"

class ConfigPanel : public wxPanel {
public:
    ConfigPanel(wxWindow* parent);
    void BindViewport(ViewportPanel* viewport) {viewport_ = viewport;}
private:
    ViewportPanel* viewport_;
};
