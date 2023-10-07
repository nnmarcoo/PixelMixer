#include "StatsPanel.h"
#include "Palette.h"
#include "ViewportPanel.h"

BEGIN_EVENT_TABLE(StatsPanel, wxPanel)
END_EVENT_TABLE()

StatsPanel::StatsPanel(wxWindow* parent, ViewportPanel* viewport) : wxPanel(parent), viewport_(viewport) {
    wxWindowBase::SetBackgroundColour(Palette::config);

    rendertime_ = new wxStaticText(this, wxID_ANY, "Render time: ", wxPoint(30, 30));
    rendertime_->SetFont(wxFont(13, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    rendertime_->SetForegroundColour(Palette::text);

    viewport_->SetStatsPanel(this);
}

void StatsPanel::UpdateRenderTime(double time) const {
    if (!IsShown()) return;
    rendertime_->SetLabel("Render time: " + std::to_string(time).substr(0, 4) + "ms");
}
