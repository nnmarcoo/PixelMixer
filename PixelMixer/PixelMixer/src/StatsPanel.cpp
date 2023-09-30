#include "StatsPanel.h"
#include "Palette.h"

BEGIN_EVENT_TABLE(StatsPanel, wxPanel)
    EVT_TIMER(wxID_ANY, StatsPanel::OnTimer)
END_EVENT_TABLE()

StatsPanel::StatsPanel(wxWindow* parent, ViewportPanel* viewport) : wxPanel(parent), viewport_(viewport) {
    wxWindowBase::SetBackgroundColour(Palette::config);

    rendertime_ = new wxStaticText(this, wxID_ANY, "Render time: " + std::to_string(viewport_->renderinms_), wxPoint(30, 30));
    rendertime_->SetFont(wxFont(13, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    rendertime_->SetForegroundColour(Palette::text);

    wxTimer* updateTimer = new wxTimer(this, wxID_ANY);
    updateTimer->Start(10);
}

void StatsPanel::OnTimer(wxTimerEvent& e) { // todo instead of using timer, call update render time from viewport
    if (!IsShown()) return;
    rendertime_->SetLabel("Render time: " + std::to_string(viewport_->renderinms_).substr(0, 4) + "ms");
}
