#pragma once
#include "OutputPanel.h"

class StatsPanel : public wxPanel {
public:
    StatsPanel(wxWindow* parent, ViewportPanel* viewport);
private:
    ViewportPanel* viewport_;
};
