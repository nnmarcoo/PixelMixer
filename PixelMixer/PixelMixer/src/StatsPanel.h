#pragma once
#include "OutputPanel.h"

class StatsPanel : public wxPanel {
public:
    StatsPanel(wxWindow* parent, ViewportPanel* viewport);
private:
    DECLARE_EVENT_TABLE()
    ViewportPanel* viewport_;
    wxStaticText* rendertime_;

    void OnTimer(wxTimerEvent& e);
};
