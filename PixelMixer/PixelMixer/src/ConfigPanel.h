#pragma once
#include "BasicSettingsPanel.h"
#include "ConfigTabs.h"

#include "ViewportPanel.h"

class ConfigPanel : public wxPanel {
public:
    ConfigPanel(wxWindow* parent);
    void BindViewport(ViewportPanel* viewport) {viewport_ = viewport; basicsettings_->BindViewport(viewport_);}
private:
    ViewportPanel* viewport_;
    BasicSettingsPanel* basicsettings_;
};
