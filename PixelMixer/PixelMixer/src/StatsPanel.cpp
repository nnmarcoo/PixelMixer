#include "StatsPanel.h"
#include "Palette.h"
#include "ViewportPanel.h"

BEGIN_EVENT_TABLE(StatsPanel, wxPanel)
END_EVENT_TABLE()

StatsPanel::StatsPanel(wxWindow* parent, ViewportPanel* viewport) : wxPanel(parent), viewport_(viewport) {
    wxWindowBase::SetBackgroundColour(Palette::config);

    wxFont font = wxFont(13, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

    rendertime_ = new wxStaticText(this, wxID_ANY, "Render time: 0.00ms", wxPoint(30, 30));
    rendertime_->SetFont(font);
    rendertime_->SetForegroundColour(Palette::text);

    zoomfactor_ = new wxStaticText(this, wxID_ANY, "Zoom factor: 100%", wxPoint(30, 60));
    zoomfactor_->SetFont(font);
    zoomfactor_->SetForegroundColour(Palette::text);

    viewport_->SetStatsPanel(this);
}

void StatsPanel::UpdateRenderTime(double time) const {
    if (!IsShown()) return;
    rendertime_->SetLabel("Render time: " + std::to_string(time).substr(0, 4) + "ms");
}

void StatsPanel::UpdateZoomFactor(double zoomfactor) const {
    if (!IsShown()) return;
    std::string string = std::to_string(zoomfactor*100);
    zoomfactor_->SetLabel("Zoom factor: " + string.substr(0, string.find('.')) + "%");
}