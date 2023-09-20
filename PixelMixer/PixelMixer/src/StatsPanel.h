#pragma once
#include "ViewportPanel.h"

class StatsPanel : public wxPanel {
public:
    StatsPanel(wxWindow* parent, ViewportPanel* viewport);
private:
    ViewportPanel* viewport_;
};
