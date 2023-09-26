#include "StatsPanel.h"
#include "Palette.h"

BEGIN_EVENT_TABLE(StatsPanel, wxPanel)
    EVT_TIMER(wxID_ANY, StatsPanel::OnTimer)
END_EVENT_TABLE()

StatsPanel::StatsPanel(wxWindow* parent, ViewportPanel* viewport) : wxPanel(parent), viewport_(viewport) {
    wxWindowBase::SetBackgroundColour(Palette::config);

    rendertime_ = new wxStaticText(this, wxID_ANY, "Render time: " + std::to_string(viewport_->renderinms_), wxPoint(30, 30));
    rendertime_->SetForegroundColour(Palette::text);

    wxTimer* updateTimer = new wxTimer(this, wxID_ANY);
    updateTimer->Start(1);

    
}

void StatsPanel::OnTimer(wxTimerEvent& e) {
    rendertime_->SetLabel("Render time: " + std::to_string(viewport_->renderinms_));
}
